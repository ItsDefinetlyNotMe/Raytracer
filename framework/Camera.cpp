#define _USE_MATH_DEFINES
#include "Camera.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include "Hitpoint.hpp"

//Camera::Camera(std::string const& name, float fov_x) :name_{ name }, fov_x_{ fov_x }, position_{ glm::vec3{} }, front_{ glm::vec3{0.0f,0.0f,-1.0f} }, up_{ glm::vec3{0.0f,1.0f,0.0f} }{}
/*
Color Camera::trace_ray(float x, float y,float aspect, std::vector<std::shared_ptr<Shape>> const& s) {
    float fov_rad = fov_x_ * (M_PI / 180.0f);

    float camera_x = x * tanf(fov_rad / 2.0f) * aspect;
    float camera_y = y * tanf(fov_rad / 2.0f);
    
    glm::vec3 ray_dir{ camera_x, camera_y, -1.0f };
    ray_dir = glm::normalize(ray_dir);
    Ray r{ position_,ray_dir };

    float closest_t = INFINITY;

    Color col = Color{};
    for (auto const& obj : s) {
        Hitpoint hitp = obj->intersect(r);
        //secondary ray
        for(auto const& light: lights){
            float closest_st = INFINITY;
            Ray sr{ hitp.point3d,glm::normalize(light - hitp.point3d) };
            for (auto const& sobj : s) {
                Hitpoint shitp = obj->intersect(r);
                if (shitp.hit && shitp.t < closest_st) {
                    col = shitp.mat->ka_;
                    closest_st = hitp.t;
                }
            }
            hitp.mat->m_ * (R(reflektierender vektor)* (/*vektor zur kamera*///r.origin + r.direction) - (hitp.point3d));
//            ambient.brigthness * hitp.mat.ka_/*ambienter reflektionskoeffizient*/ + light.brightness * hitp.mat * (NORMALENVEKTOR * sr.direction);
  /*      if (hitp.hit && hitp.t < closest_t) {
            col = hitp.mat->ka_;
            closest_t = hitp.t;
        }
    }
    return col;
}*/