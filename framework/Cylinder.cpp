#define _USE_MATH_DEFINES
#include "Cylinder.hpp"
#include "helper.hpp"
#include <glm/gtx/intersect.hpp>

Cylinder::Cylinder(std::string const& n,std::shared_ptr<Material> const& m, glm::vec3 b, float r, float h):Shape{ n,m },bottom_ { b }, radius_{ r }, height_{ h } {};

float Cylinder::area() const {
	return 2 * M_PI * radius_ * height_;
}

float Cylinder::volume() const {
	return 2 * M_PI * radius_ * height_;
}

std::ostream& Cylinder::print(std::ostream& os) const {
	return Shape::print(os) << "position: " << "(" << bottom_.x << ", " << bottom_.y << ", " << bottom_.z << ")" << " radius: " << radius_ << ", height: " << height_;
}

bool intersect_2d_circle(glm::vec3& x1, Ray ray, float radius) {
	ray.origin.z = 0;
	ray.direction.z = 0;

	float a = pow(glm::length(ray.direction), 2);
	if (a == 0.0f)
		return false;
	float b = 2 * (ray.origin.x * ray.direction.x - ray.origin.y * ray.direction.y);
	float c = pow(glm::length(ray.origin), 2) - radius * radius;
	float root = b * b - 4 * a * c;
	if (root < 0.0f)
		return false;
	root = sqrt(root);
	if (root == -root) {
		x1 = ((-b + root) / 2 * a) * ray.direction + ray.origin;
		return true;
	}
	//float tx1 = ((-b + root) / 2 * a);
	//float tx2 = ((-b - root) / 2 * a);
	/*if (abs(tx1) < abs(tx2) && tx1 > 0) {
		x1 = ((-b + root) / 2 * a) * ray.direction + ray.origin;
		return true;
	}
	else if (tx2 > 0) {
		x1 = ((-b - root) / 2 * a) * ray.direction + ray.origin;
		return true;
	}
	return false;*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glm::vec3 tx1 = ((-b + root) / 2 * a) * ray.direction + ray.origin;
	glm::vec3 tx2 = ((-b - root) / 2 * a) * ray.direction + ray.origin;
	
	if (glm::length(tx1 - ray.origin) >= glm::length(tx2 - ray.origin)) {
		x1 = tx2;
		return true;
	}
	x1 = tx2;
}

Hitpoint Cylinder::intersect(Ray const& ray) const {
	Ray obj_ray = { world_to_obj_position(ray.origin), glm::normalize(world_to_obj_direction(ray.direction)) };
	/*
	glm::vec3 point;
	if (intersect_2d_circle(point, obj_ray, radius_)) {
		if (glm::length(point) < radius_)
			std::cout << point.x << point.y << "\n";
		return Hitpoint{ true, 12 ,Shape::get_name(),Shape::get_material(), point,glm::normalize(ray.direction) };
	}
	return Hitpoint{};*/
	Hitpoint point{};
	float disb = ((bottom_.z - obj_ray.origin.z) / obj_ray.direction.z);
	float world_disb = disb * Shape::get_scale();
	glm::vec3 pb = obj_ray.origin + disb * obj_ray.direction;

	float dist = ((bottom_.z + height_ - obj_ray.origin.z) / obj_ray.direction.z);
	float world_dist = dist * Shape::get_scale();
	glm::vec3 pt = obj_ray.origin + dist * obj_ray.direction;
	

	if (glm::length(pt - glm::vec3{bottom_.x, bottom_.y, bottom_.z + height_}) <= radius_) {
		if (world_dist < point.t && world_dist >= 0)
			point = { true, world_dist,Shape::get_name(),Shape::get_material(), Shape::obj_to_world_position(pt) ,glm::normalize(Shape::obj_to_world_direction(obj_ray.direction)) };
	} 
	if (glm::length(pb - bottom_) <= radius_) {
		if (world_disb < point.t && world_disb >= 0)
			point = { true, world_disb ,Shape::get_name(),Shape::get_material(),Shape::obj_to_world_position(pb) ,glm::normalize(Shape::obj_to_world_direction(obj_ray.direction)) };
	}
	
	//2d as circle
	glm::vec2 ray_dir_2d{ obj_ray.direction.x,obj_ray.direction.y };
	ray_dir_2d = glm::normalize(ray_dir_2d);
	glm::vec2 U{ (bottom_ - obj_ray.origin).x,(bottom_ - obj_ray.origin).y };
	glm::vec2 U1 = glm::dot(U, ray_dir_2d) * ray_dir_2d;
	glm::vec2 U2{ U - U1 };

	float d = glm::length(U2);
	if (d > radius_)
		return Hitpoint{};

	float m = sqrt(radius_ * radius_ - d * d);
	glm::vec2 p2 = glm::vec2{ obj_ray.origin.x,obj_ray.origin.y } + U1 + m * ray_dir_2d;
	glm::vec2 p1 = glm::vec2{ obj_ray.origin.x,obj_ray.origin.y } + U1 - m * ray_dir_2d;
	//
	float t2 =  (p2.x - obj_ray.origin.x) / obj_ray.direction.x;
	float t1 = (p1.x - obj_ray.origin.x) / obj_ray.direction.x;

	float t = t2;
	if (t1 < t2 && t1 >= 0 || t2 < 0)
		t = t1;

	glm::vec3 point_hit = obj_ray.origin + obj_ray.direction * t1;
	if (point_hit.z <= bottom_.z + height_ && point_hit.z >= bottom_.z){
		float world_t = t * Shape::get_scale();
		glm::vec3 world_point_hit{ Shape::obj_to_world_position(obj_ray.origin) + glm::normalize(Shape::obj_to_world_direction(obj_ray.direction * world_t)) };
		if(world_t < point.t && point.t > 0 )
			point = { true, t ,Shape::get_name(),Shape::get_material(), world_point_hit,glm::normalize(Shape::obj_to_world_direction(obj_ray.direction)) };
	}
	return point;
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
	return glm::vec3{  };
}