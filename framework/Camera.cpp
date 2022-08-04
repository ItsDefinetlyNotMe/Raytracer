#include "Camera.hpp"
Camera::Camera(std::string const& name, float fov_x) :name_{ name }, fov_x_{ fov_x }, position_{ glm::vec3{} }, front_{ glm::vec3{0.0f,0.0f,-1.0f} }, up_{ glm::vec3{0.0f,1.0f,0.0f} }{}
