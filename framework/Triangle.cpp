#include "Triangle.hpp"
#include <glm/gtx/intersect.hpp>
Triangle::Triangle(glm::vec3 lb, glm::vec3 rb, glm::vec3 t) :left_bottom_{ lb }, right_bottom_{ rb }, top_{ t } {};

float Triangle::volume() const {
	return 0.0f;
}

float Triangle::area() const {
	return (abs(right_bottom_.z - top_.z) * (left_bottom_.y - right_bottom_.y)) / 2;
}


std::ostream& Triangle::print(std::ostream& os) const {
	return Shape::print(os) << "vertecie 0: (" << left_bottom_.x << "," << left_bottom_.y << "," << left_bottom_.z << ") " << "vertecie 1: (" << right_bottom_.x << "," << right_bottom_.y << "," << right_bottom_.z << ")" << "vertecie 2: (" << top_.x << "," << top_.y << "," << top_.z << ")";
}

Hitpoint Triangle::intersect(Ray const& r) const {
	glm::vec3 point{};
	Ray obj_ray = { world_to_obj_position(r.origin),glm::normalize(world_to_obj_direction(r.direction)) };
	bool hit = glm::intersectRayTriangle(r.origin, r.direction, right_bottom_, left_bottom_, top_, point);
	glm::vec3 world_point = point * Shape::get_scale();//Sure ?
	return Hitpoint{hit,glm::length(world_point - r.origin),Shape::get_name(),Shape::get_material(),world_point,r.direction};
}

glm::vec3 Triangle::normal(glm::vec3 const& point) const {
	return glm::normalize(obj_to_world_direction(glm::cross(left_bottom_ - top_,right_bottom_ - top_)));
}