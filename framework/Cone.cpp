#include "Cone.hpp"
#include "helper.hpp"
float Cone::area() const {
	return 0;
}

float Cone::volume() const {
	return 0;
}

std::ostream& Cone::print(std::ostream& os) const {
	return Shape::print(os) << "position: " << "(" << bottom_.x << ", " << bottom_.y << ", " << bottom_.z << ")" << " radius: " << radius_ << ", height: " << height_;
}

Hitpoint Cone::intersect(Ray const& ray) const {
	Ray obj_ray = { world_to_obj_position(ray.origin), glm::normalize(world_to_obj_direction(ray.direction)) };

	glm::vec2 ray_dir_2d{ obj_ray.direction.x,obj_ray.direction.y };
	glm::vec2 U{ (bottom_ - obj_ray.origin).x,(bottom_ - obj_ray.origin).y };
	glm::vec2 U1 = glm::dot(U, ray_dir_2d) * ray_dir_2d;
	glm::vec2 U2{ U - U1 };

	float d = glm::length(U2);
	if (d > radius_)
		return Hitpoint{};

	float m = sqrt(radius_ * radius_ - d * d);
	glm::vec2 p2 = glm::vec2{ obj_ray.origin.x,obj_ray.origin.y } + U1 - m * ray_dir_2d;
	//
	float t = (p2.x - obj_ray.origin.x) / ray_dir_2d.x;
	glm::vec3 point_hit = obj_ray.origin + obj_ray.direction * t;

	float r = radius_ - (radius_ / height_) * abs((bottom_.z - point_hit.z));
	if (point_hit.z >= bottom_.z && point_hit.z <= bottom_.z + height_) {
		if (glm::length(glm::vec2{ bottom_.x,bottom_.y } - p2) <= r) {
			glm::vec3 world_point_hit{Shape::obj_to_world_position(point_hit)};
			return Hitpoint{ true, t ,Shape::get_name(),Shape::get_material(), world_point_hit,glm::normalize(ray.direction) };
		}
	}
}

glm::vec3 Cone::normal(glm::vec3 const& point) const {
	glm::vec3 obj_point = Shape::world_to_obj_position(point);
	if (floating_equal<float>(obj_point.z, bottom_.z))
		return glm::normalize(obj_to_world_direction({ 0.0f,0.0f,-1.0f }));
	glm::vec3 norm{ obj_point - bottom_ };
	norm.z = height_/radius_;
	return glm::normalize(obj_to_world_direction(norm));
}