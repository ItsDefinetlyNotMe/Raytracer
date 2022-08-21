#define _USE_MATH_DEFINES
#include "Cylinder.hpp"
#include "helper.hpp"
#include <glm/gtx/intersect.hpp>
float Cylinder::area() const {
	return 2 * M_PI * radius_ * height_;
}

float Cylinder::volume() const {
	return 2 * M_PI * radius_ * height_;
}

std::ostream& Cylinder::print(std::ostream& os) const {
	return Shape::print(os) << "position: " << "(" << bottom_.x << ", " << bottom_.y << ", " << bottom_.z << ")" << " radius: " << radius_ << ", height: " << height_;
}

Hitpoint Cylinder::intersect(Ray const& ray) const {
	Ray obj_ray = { world_to_obj_position(ray.origin), glm::normalize(world_to_obj_direction(ray.direction)) };
	
	//2d as circle:
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
	if (point_hit.z <= bottom_.z + height_ && point_hit.z >= bottom_.z)
	{
		float world_t = t * Shape::get_scale();
		glm::vec3 world_point_hit{ ray.origin + glm::normalize(ray.direction) * world_t };
		return Hitpoint{ true, t ,Shape::get_name(),Shape::get_material(), world_point_hit,glm::normalize(ray.direction) };
	}
	return Hitpoint{};
}

glm::vec3 Cylinder::normal(glm::vec3 const& point) const {
	glm::vec3 obj_point = Shape::world_to_obj_position(point);
	//oben/unten
	if (floating_equal<float>(obj_point.z, bottom_.z)) {
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3{0.0f,0.0f,-1.0f}));
	}
	else if(floating_equal<float>(obj_point.z, bottom_.z + height_)) {
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3{ 0.0f,0.0f,1.0f }));
	}
	//seite
	float distance = glm::length(glm::vec2{ obj_point.x ,obj_point.y });
	if (floating_equal<float>(distance,radius_)) {
		glm::vec3 normal = glm::vec3{ obj_point.x,point.y,bottom_.z } - bottom_;
		return glm::normalize(Shape::obj_to_world_direction(normal));
	}

}