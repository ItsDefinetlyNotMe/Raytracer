#include "Cone.hpp"
#include "helper.hpp"

Cone::Cone(std::string const& n, std::shared_ptr<Material> const& m, glm::vec3 const& b, float r, float h) :Shape{n,m},bottom_ { b }, radius_{ r }, height_{ h } {}
Cone::~Cone() {
	std::cout << "Destructor Cone";
};
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

	Hitpoint point{};
	float disb = ((bottom_.z - obj_ray.origin.z) / obj_ray.direction.z);//bottom.z = obj_ray-origin.z + disb * obj_ray.direction.z
	float world_disb = disb * Shape::get_scale();
	glm::vec3 pb = obj_ray.origin + disb * obj_ray.direction;

	if (glm::length(pb - bottom_) <= radius_) {
		if (world_disb >= 0.0f)
				point =  { true, world_disb ,Shape::get_name(),Shape::get_material(),Shape::obj_to_world_position(pb) ,glm::normalize(Shape::obj_to_world_direction(obj_ray.direction)) };
	}
	//
	glm::vec3 h_dach{ glm::vec3{0.0f,0.0f,bottom_.z - height_} / abs(bottom_.z - height_) };
	glm::vec3 w{ obj_ray.origin - glm::vec3{bottom_.x,bottom_.y,bottom_.z + height_} };
	float m = radius_ * radius_/pow(glm::length((glm::vec3{0.0f,0.0f,height_})),2);

	float a = glm::dot(obj_ray.direction, obj_ray.direction) - m * pow(glm::dot(obj_ray.direction, h_dach), 2) - pow(glm::dot(obj_ray.direction, h_dach), 2);
	float b = 2 * (glm::dot(obj_ray.direction, w) - m * glm::dot(obj_ray.direction, h_dach) * glm::dot(w, h_dach) - glm::dot(obj_ray.direction, h_dach) * glm::dot(w, h_dach));
	float c = glm::dot(w, w) - m * pow(glm::dot(w, h_dach), 2) - pow(glm::dot(w, h_dach), 2);
	
	if (b * b - 4 * a * c < 0)
		return Hitpoint{};
	float t1 = (-b + sqrt(b*b -4 * a * c)) / (2 * a);
	float t2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	
	if (t1 <= t2 && t1 > 0) {

		if (t1 * Shape::get_scale() < point.t) {
			if ((obj_ray.origin + t1 * obj_ray.direction).z > bottom_.z && (obj_ray.origin + t1 * obj_ray.direction).z <= bottom_.z + height_)
				point = { true, t1 * Shape::get_scale() ,Shape::get_name(),Shape::get_material(),Shape::obj_to_world_position(obj_ray.origin + t1 * obj_ray.direction) ,glm::normalize(Shape::obj_to_world_direction(obj_ray.direction)) };
		}
	}
	if (t2 <= t1 && t2 > 0) {
		
		if (t2 * Shape::get_scale() < point.t) {
			if ((obj_ray.origin + t2 * obj_ray.direction).z > bottom_.z && (obj_ray.origin + t2 * obj_ray.direction).z <= bottom_.z + height_)
				point = { true, t2 * Shape::get_scale() ,Shape::get_name(),Shape::get_material(),Shape::obj_to_world_position(obj_ray.origin + t2 * obj_ray.direction) ,glm::normalize(Shape::obj_to_world_direction(obj_ray.direction)) };
		}
	}

	return point;
}

glm::vec3 Cone::normal(glm::vec3 const& point) const {
	glm::vec3 obj_point = Shape::world_to_obj_position(point);
	if (floating_equal<float>(obj_point.z, bottom_.z))
		return glm::normalize(obj_to_world_direction({ 0.0f,0.0f,-1.0f }));
	glm::vec3 norm{ obj_point - bottom_ };
	norm.z = height_/radius_;
	return glm::normalize(obj_to_world_direction(norm));
}