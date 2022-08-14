// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include "renderer.hpp"
#include <glm/gtx/rotate_vector.hpp>
//added Camera to the mix
//wenn es per const& übergebn werden brauchen wir keinen pointer oder ?
Renderer::Renderer(unsigned w, unsigned h, std::string const& file,Camera const& cam,Scene const& sce)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , camera_(cam)
  , scene_(sce)
  , ppm_(width_, height_)
{}

void Renderer::render()
{
    for (unsigned x = 0; x < width_; ++x) {
        for (unsigned y = 0; y < height_; ++y) {
            Pixel p(x, y);

            
            float screen_x_normalized = 2.0f * (((float)x + 0.5f) / (float)width_) - 1.0f;
            float screen_y_normalized = 2.0f * (((float)y + 0.5f) / (float)height_) - 1.0f;

            p.color = trace_ray(screen_x_normalized,screen_y_normalized);
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

    glm::vec3 ray_dir{ camera_x, camera_y, -1.0f };
    ray_dir = glm::normalize(ray_dir);
    Ray r{ camera_.position_,ray_dir };
    Hitpoint closest_p;
    closest_p.t = INFINITY;

    std::shared_ptr<Shape>object_hit;

    Color col = Color{0.0f,0.0f,0.0f};
    /////////////////////////////////////////////
    for (auto const& obj : scene_.world_) {
        Hitpoint hitp = obj->intersect(r);
        if (hitp.hit && hitp.t < closest_p.t) {
            closest_p = hitp;
            object_hit = obj;
        }
    }
    //secondary ray
    if (closest_p.hit) 
        col += lightning(closest_p, object_hit);
    
    return col;
}

Color Renderer::lightning(Hitpoint const& h,std::shared_ptr<Shape> const& obj_h) const {
    Color col{ scene_.ambient_ * h.mat->ka_ };
    //ambient
    float epsilon = 0.001f;
    for (auto const& light : scene_.lights_){
        bool obstruction = false;
        //mit einem epsilon ?
        Ray secondary_ray{ h.point3d + (obj_h->normal(h.point3d) * 0.1f)  ,glm::normalize(light->position_ - h.point3d)};
        //
        for (auto const& sobj : scene_.world_) {
            Hitpoint shitp = sobj->intersect(secondary_ray);
                if (shitp.hit && shitp.t >= 0) {
                    obstruction = true;
                    break;
                }
        }
        if (!obstruction) {
            //diffuse
            glm::vec3 norm{ obj_h->normal(h.point3d) };
            float idk = glm::dot(norm, secondary_ray.direction);
            //idk wird negativ was tun ?
            col += light->color_ * light->brightness_ * h.mat->kd_ * std::max(idk,0.0f);
            
            //specular //!!Verhält sich komisch bei m = 1
            glm::vec3 rotated_i{ (-secondary_ray.direction) - (2 * glm::dot(-secondary_ray.direction,norm) * norm) };
            glm::vec3 eye_vec{ glm::normalize(h.point3d - camera_.position_)};

            col += std::max(std::pow(glm::dot(rotated_i, eye_vec), h.mat->m_), 0.0f) * h.mat->ks_ * light->brightness_ * light->color_;
        }
    }
        return col;
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
