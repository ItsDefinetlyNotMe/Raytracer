// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------
#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#include "renderer.hpp"
#include <glm/gtx/rotate_vector.hpp>
//added Camera to the mix
//wenn es per const& �bergebn werden brauchen wir keinen pointer oder ?
Renderer::Renderer(unsigned w, unsigned h, std::string const& file,Camera const& cam,Scene const& sce)
    : width_(w)
    , height_(h)
    , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
    , filename_(file)
    , camera_(cam)
    , scene_(sce)
    , ppm_(width_, height_) {}

void Renderer::render()
{
    float d = sin((camera_.fov_x_ / 2.0f) * (M_PI / 180.0f));
    float distance = ((float)width_ / 2.0f) / d;

    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            Pixel p(x, y);

            // offset by 0.5f to shoot through the middle of the pixel
            float screen_x = (float) x - ((float) width_ / 2.0f) + 0.5f;
            float screen_y = (float) y - ((float) height_ / 2.0f) + 0.5f;

            Ray prim_ray {glm::vec3(0, 0, 0), glm::normalize(glm::vec3{screen_x, screen_y, -distance})};

            glm::mat4 view = glm::lookAt(camera_.position_, camera_.position_ + camera_.front_, camera_.up_);
            glm::mat4 inv_view = glm::inverse(view);

            prim_ray.origin = glm::vec3(inv_view * glm::vec4(prim_ray.origin, 1.0f));
            prim_ray.direction = glm::vec3(inv_view * glm::vec4(prim_ray.direction, 0.0f));

            p.color = trace_primary(prim_ray);
            p.color = Color{ p.color.r / (p.color.r + 1),p.color.g / (p.color.g + 1),p.color.b / (p.color.b + 1) };
            write(p);
        }
    }
    ppm_.save(filename_);
}

Color Renderer::trace_primary(Ray const& prim_ray) const {
    Color pixel_color = Color{ 0.0f,0.0f,0.0f };

    Hitpoint hitp = scene_.root_->intersect(prim_ray);

    //secondary ray
    if (hitp.hit && hitp.t > 0.0f) {
        pixel_color += trace_secondary(hitp, 1, 0);
        if (hitp.mat->reflectivity_ > 0.0f)
            pixel_color += trace_secondary(hitp, 1, 1) * hitp.mat->reflectivity_;
        if (hitp.mat->opacity_ < 1.0f)
            pixel_color += trace_secondary(hitp, 1, 2) * (1.0f - hitp.mat->opacity_); 
    }

    return pixel_color;
    return Color{};
}

Color Renderer::trace_secondary(Hitpoint const& hitpoint, unsigned int depth, unsigned int type) const {
    if (depth > 4) return Color{0.0f, 0.0f, 0.0f};

    Color pixel_color{0.0f, 0.0f, 0.0f};

    //ambient
    if (type == 0)
        pixel_color = Color{ scene_.ambient_ * hitpoint.mat->ka_ };

    float epsilon = 0.0005f;

    if (type == 0) {
        for (auto const& light : scene_.lights_){
            bool obstructed = false;

            glm::vec3 offset_hitpoint = hitpoint.point3d + hitpoint.normal * epsilon;
            Ray secondary_ray {offset_hitpoint, glm::normalize(light->position_ - offset_hitpoint)};

            Hitpoint hitp = scene_.root_->intersect(secondary_ray);
            if (hitp.hit && hitp.t < glm::length(light->position_ - offset_hitpoint)) {
                obstructed = true;
            }

            if (!obstructed) {
                //diffuse
                glm::vec3 normal_object_hit_n{ hitpoint.normal };
                float cos_omega = glm::dot(normal_object_hit_n, secondary_ray.direction);
                pixel_color += light->color_ * light->brightness_ * hitpoint.mat->kd_ * std::max(cos_omega,0.0f);
                
                //specular 
                glm::vec3 reflected_i_n{ (-secondary_ray.direction) - (2 * glm::dot(-secondary_ray.direction,normal_object_hit_n) * normal_object_hit_n) };
                glm::vec3 vec_to_cam_n = -hitpoint.direction;
                pixel_color += std::pow(std::max(glm::dot(reflected_i_n, vec_to_cam_n), 0.0f), hitpoint.mat->m_) * hitpoint.mat->ks_ * light->brightness_ * light->color_;
            }
        }
    }

    if (type == 1) {
        glm::vec3 offset_hitpoint = hitpoint.point3d + hitpoint.normal * epsilon;
        Ray secondary_ray {offset_hitpoint, hitpoint.direction - 2.0f * glm::dot(hitpoint.direction, hitpoint.normal) * hitpoint.normal};

        Hitpoint hitp = scene_.root_->intersect(secondary_ray);
        if (hitp.hit && hitp.t > 0.0f) {
            pixel_color += trace_secondary(hitp, depth, 0);
            if (hitp.mat->reflectivity_ > 0.0f)
                pixel_color += trace_secondary(hitp, depth+1, 1) * hitp.mat->reflectivity_;
            if (hitp.mat->opacity_ < 1.0f)
                pixel_color += trace_secondary(hitp, depth+1, 2) * (1.0f - hitp.mat->opacity_); 
        }
    } else if (type == 2) {
        // Snell's law
        glm::vec3 norm = hitpoint.normal;
        float eta_1 = 1.0f;
        float eta_2 = hitpoint.mat->refractive_index_;
        float cos_i = glm::dot(norm, hitpoint.direction);
        if (cos_i < 0.0f) {
            // we are outside
            // we want our cos to be positive
            cos_i = -cos_i;
        } else {
            // we are inside
            std::swap(eta_1, eta_2);
            // we want our normal to look inside
            norm = -norm;
        }

        float eta = (eta_1 / eta_2);
        float sin2_t = (eta * eta) * (1.0f - (cos_i * cos_i));
        float cos_t;

        float refl;
        float trans;

        if (sin2_t > 1.0f) {
            // TIR
            trans = 0.0f;
            refl = 1.0f;
        } else {
            cos_t = sqrtf(1.0f - sin2_t);
            float Fren_O = ((eta_1 * cos_i - eta_2 * cos_t) / (eta_1 * cos_i + eta_2 * cos_t))
                         * ((eta_1 * cos_i - eta_2 * cos_t) / (eta_1 * cos_i + eta_2 * cos_t));
            float Fren_P = ((eta_2 * cos_i - eta_1 * cos_t) / (eta_2 * cos_i + eta_1 * cos_t))
                         * ((eta_2 * cos_i - eta_1 * cos_t) / (eta_2 * cos_i + eta_1 * cos_t));

            refl = (Fren_O + Fren_P) / 2.0f;
            trans = 1.0f - refl;
        }
        
        Hitpoint transmitted;
        Hitpoint refracted;

        if (trans > 0.0f) {
            glm::vec3 offset_hitpoint = hitpoint.point3d - norm * epsilon;
            glm::vec3 trans_dir = eta * hitpoint.direction + (eta * cos_i - cos_t) * (norm);
            Ray secondary_ray {offset_hitpoint, trans_dir};

            Color trans_color {0.0f, 0.0f, 0.0f};

            Hitpoint hitp = scene_.root_->intersect(secondary_ray);
            if (hitp.hit && hitp.t > 0.0f) {
                trans_color += trace_secondary(hitp, depth, 0);
                if (hitp.mat->reflectivity_ > 0.0f)
                    trans_color += trace_secondary(hitp, depth+1, 1) * hitp.mat->reflectivity_;
                if (hitp.mat->opacity_ < 1.0f)
                    trans_color += trace_secondary(hitp, depth+1, 2) * (1.0f - hitp.mat->opacity_); 
            }
            pixel_color += trans_color * trans;
        }

        if (refl > 0.0f) {
            glm::vec3 offset_hitpoint = hitpoint.point3d + norm * epsilon;
            glm::vec3 refl_dir = hitpoint.direction - 2.0f * glm::dot(hitpoint.direction, norm) * norm;
            Ray secondary_ray {offset_hitpoint, refl_dir};

            Color refl_color {0.0f, 0.0f, 0.0f};

            Hitpoint hitp = scene_.root_->intersect(secondary_ray);
            if (hitp.hit && hitp.t > 0.0f) {
                refl_color += trace_secondary(hitp, depth, 0);
                if (hitp.mat->reflectivity_ > 0.0f)
                    refl_color += trace_secondary(hitp, depth+1, 1) * hitp.mat->reflectivity_;
                if (hitp.mat->opacity_ < 1.0f)
                    refl_color += trace_secondary(hitp, depth+1, 2) * (1.0f - hitp.mat->opacity_); 
            }
            pixel_color += refl_color * refl;
        }
    }
    return pixel_color;
}

void Renderer::write(Pixel const& p){

    // flip pixels, because of opengl glDrawPixels
    size_t buf_pos = (width_*p.y + p.x);
    if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
        std::cerr << "Fatal Error Renderer::write(Pixel p) : "
            << "pixel out of ppm_ : "
            << (int)p.x << "," << (int)p.y
            << std::endl;
    } else {
        color_buffer_[buf_pos] = p.color;
    }

    ppm_.write(p);
}
