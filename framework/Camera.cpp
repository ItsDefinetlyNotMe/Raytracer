#define _USE_MATH_DEFINES
#include "Camera.hpp"

#include <cmath>
#include <glm/glm.hpp>

#include "pixel.hpp"
#include "Hitpoint.hpp"
#include "Scene.hpp"

Camera::Camera(std::string const& name, float fov_x) :name_{ name }, fov_x_{ fov_x }, position_{ glm::vec3{} }, front_{ glm::vec3{0.0f,0.0f,-1.0f} }, up_{ glm::vec3{0.0f,1.0f,0.0f} }{}

Color Camera::trace_ray(Pixel const p,float aspect,Scene const& s) {
    float fov_rad = fov_x_ * (M_PI / 180.0f);

    float camera_x = p.x * tanf(fov_rad / 2.0f) * aspect;
    float camera_y = p.y * tanf(fov_rad / 2.0f);
    
    glm::vec3 ray_dir{ camera_x, camera_y, -1.0f };
    ray_dir = glm::normalize(ray_dir);


    float closest_t = INFINITY;
    Color col = Color{};
    for (auto const& obj : s.world_) {
        Hitpoint hitp = obj->intersect(ray_dir);
        if (hitp.hit && hitp.t < closest_t) {
            col = hitp.mat->ka_;
            closest_t = hitp.t;
        }
    }
    return col;
}