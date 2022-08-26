#include "Cone.hpp"
#include "helper.hpp"

Cone::Cone(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 b, float r, float h) : Shape(n, mat), bottom_(b), radius_(r), height_(h) {}

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
	
	// cone is defined as x^2+y^2=z^2 , so we need a slope
	float slope = radius_ / height_;

	// we need to offset the cone origin to (0, 0, 0)
	obj_ray.origin = obj_ray.origin - bottom_ - glm::vec3{0, height_, 0};

	// solve as unit cone
	float a = obj_ray.direction.x * obj_ray.direction.x + 
			  obj_ray.direction.z * obj_ray.direction.z -
			  obj_ray.direction.y * obj_ray.direction.y * slope * slope;

	float b = 2 * (obj_ray.origin.x * obj_ray.direction.x + 
				   obj_ray.origin.z * obj_ray.direction.z -
				   obj_ray.origin.y * obj_ray.direction.y * slope * slope);

	float c = obj_ray.origin.x * obj_ray.origin.x + 
			  obj_ray.origin.z * obj_ray.origin.z - 
			  obj_ray.origin.y * obj_ray.origin.y * slope * slope;

	float t1 = (-b - sqrtf(b*b - 4 * a * c)) / (2 * a);
	float t2 = (-b + sqrtf(b*b - 4 * a * c)) / (2 * a);

	float t;
	if (t2 < t1) {
		float temp = t1;
		t1 = t2;
		t2 = temp;
	}
	if (t1 >= 0.0f) t = t1;
	else if (t2 >= 0.0f) t = t2;
	else return Hitpoint{};

	// check if inside y-range
	glm::vec3 point_hit = obj_ray.origin + obj_ray.direction * t;
	if (point_hit.y <= 0.0f && point_hit.y >= -height_) {
		float world_t = t * Shape::get_scale();
		glm::vec3 world_point_hit{ ray.origin + glm::normalize(ray.direction) * world_t };
		return Hitpoint{ true, world_t ,Shape::get_name(),Shape::get_material(), world_point_hit,glm::normalize(ray.direction), normal(world_point_hit)};
	} else {
		float t3 = -obj_ray.origin.y / obj_ray.direction.y;
		float t4 = (-height_ - obj_ray.origin.y) / obj_ray.direction.y;
		if (t3 < t4 && t3 > 0.0f && t3 > t1 && t3 < t2) t = t3;
		else if (t4 > 0.0f && t4 > t1 && t4 < t2) t = t4;
		else return Hitpoint{};
		
		float world_t = t * Shape::get_scale();
		glm::vec3 world_point_hit{ ray.origin + glm::normalize(ray.direction) * world_t };
		return Hitpoint{ true, world_t ,Shape::get_name(),Shape::get_material(), world_point_hit,glm::normalize(ray.direction), normal(world_point_hit)};
	}
}

glm::vec3 Cone::normal(glm::vec3 const& point) const {
	glm::vec3 obj_point = Shape::world_to_obj_position(point);
	if (floating_equal<float>(obj_point.y, bottom_.y))
		return glm::normalize(obj_to_world_direction({ 0.0f, -1.0f, 0.0f }));
	glm::vec3 norm{ obj_point - bottom_ };
	norm.y = 0.0f;
	norm = glm::normalize(norm);
	norm.x = norm.x * (height_ / radius_);
	norm.z = norm.z * (height_ / radius_);
	norm.y = radius_ / height_;
	return glm::normalize(obj_to_world_direction(norm));
}

Bounding_Box Cone::create_bounding_box() {
	// get all 8 transformed points and build bb from there

	glm::vec3 min = bottom_ - glm::vec3(radius_, 0.0f, radius_);
	glm::vec3 max = bottom_ + glm::vec3(radius_, height_, radius_);

	glm::vec3 points[8]; // c-style array should be fine
	points[0] = obj_to_world_position(min);
	points[1] = obj_to_world_position(glm::vec3(min.x, min.y, max.z));
	points[2] = obj_to_world_position(glm::vec3(min.x, max.y, min.z));
	points[3] = obj_to_world_position(glm::vec3(min.x, max.y, max.z));
	points[4] = obj_to_world_position(glm::vec3(max.x, min.y, min.z));
	points[5] = obj_to_world_position(glm::vec3(max.x, min.y, max.z));
	points[6] = obj_to_world_position(glm::vec3(max.x, max.y, min.z));
	points[7] = obj_to_world_position(max);

	for (int i = 0; i < 8; ++i) {
		if (min.x > points[i].x) min.x = points[i].x;
		if (min.y > points[i].y) min.y = points[i].y;
		if (min.z > points[i].z) min.z = points[i].z;
		if (max.x < points[i].x) max.x = points[i].x;
		if (max.y < points[i].y) max.y = points[i].y;
		if (max.z < points[i].z) max.z = points[i].z;
	}

	Bounding_Box bb {min, max};

	Shape::set_bounding_box(bb);
	return bb;
}