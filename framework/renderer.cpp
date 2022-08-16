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
  , ppm_(width_, height_)
{}

#include <glm/gtx/string_cast.hpp>

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

            p.color = trace_ray_second(prim_ray);
            p.color = Color{ p.color.r / (p.color.r + 1),p.color.g / (p.color.g + 1),p.color.b / (p.color.b + 1) };
            write(p);
        }
    }
  ppm_.save(filename_);
}

Color Renderer::trace_ray(float x, float y) const {

    float fov_rad = camera_.fov_x_ * (M_PI / 180.0f);

    float camera_x = x * tanf(fov_rad / 2.0f) * ((float)width_ / (float)height_);
    float camera_y = y * tanf(fov_rad / 2.0f);

    glm::vec3 prim_ray_dir_n{ glm::normalize(glm::vec3{camera_x, camera_y, -1.0f }) };
    Ray prim_ray{ camera_.position_,prim_ray_dir_n };
    Hitpoint closest_hitpoint;
    closest_hitpoint.t = INFINITY;

    std::shared_ptr<Shape>object_hit;

    Color pixel_color = Color{0.0f,0.0f,0.0f};
    /////////////////////////////////////////////
    for (auto const& objects : scene_.world_) {
        Hitpoint hitp = objects->intersect(prim_ray);
        if (hitp.hit && hitp.t < closest_hitpoint.t) {
            closest_hitpoint = hitp;
            object_hit = objects;
        }
    }
    //secondary ray
    if (closest_hitpoint.hit) 
        pixel_color += lightning(closest_hitpoint, object_hit);
    
    return pixel_color;
}

Color Renderer::trace_ray_second(Ray const& prim_ray) const {
    Hitpoint closest_hitpoint;
    closest_hitpoint.t = INFINITY;

    std::shared_ptr<Shape>object_hit;

    Color pixel_color = Color{ 0.0f,0.0f,0.0f };
    /////////////////////////////////////////////
    for (auto const& objects : scene_.world_) {
        Hitpoint hitp = objects->intersect(prim_ray);
        if (hitp.hit && hitp.t < closest_hitpoint.t) {
            closest_hitpoint = hitp;
            object_hit = objects;
        }
    }
    //secondary ray
    if (closest_hitpoint.hit)
        pixel_color += lightning(closest_hitpoint, object_hit);

    return pixel_color;
    return Color{};
}

Color Renderer::lightning(Hitpoint const& hitpoint,std::shared_ptr<Shape> const& object_hit) const {
    Color pixel_color{ scene_.ambient_ * hitpoint.mat->ka_ };
    //ambient
    float epsilon = 0.0005f;
    for (auto const& light : scene_.lights_){
        bool obstructed = false;
        glm::vec3 offset_hitpoint = hitpoint.point3d + object_hit->normal(hitpoint.point3d) * epsilon;
        Ray secondary_ray {offset_hitpoint, glm::normalize(light->position_ - offset_hitpoint)};
        for (auto const& object : scene_.world_) {
            Hitpoint hitp = object->intersect(secondary_ray);
            // für innenräume müssen wir checken das die distanz zum licht größer ist als zum hindernis
            if (hitp.hit && hitp.t < glm::length(light->position_ - offset_hitpoint)) {
                obstructed = true;
                break;
            }
        }
        if (!obstructed) {
            //diffuse
            glm::vec3 normal_object_hit_n{ object_hit->normal(hitpoint.point3d) };
            float cos_omega = glm::dot(normal_object_hit_n, secondary_ray.direction);
            pixel_color += light->color_ * light->brightness_ * hitpoint.mat->kd_ * std::max(cos_omega,0.0f);
            
            //specular 
            glm::vec3 reflected_i_n{ (-secondary_ray.direction) - (2 * glm::dot(-secondary_ray.direction,normal_object_hit_n) * normal_object_hit_n) };
            glm::vec3 vec_to_cam_n{ glm::normalize(camera_.position_ - hitpoint.point3d )};
            pixel_color += std::pow(std::max(glm::dot(reflected_i_n, vec_to_cam_n), 0.0f), hitpoint.mat->m_) * hitpoint.mat->ks_ * light->brightness_ * light->color_;
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
