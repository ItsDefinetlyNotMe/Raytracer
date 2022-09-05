#define _USE_MATH_DEFINES
#include "Sphere.hpp"
#include <glm/gtx/intersect.hpp>

Sphere::Sphere(glm::vec3 const& c, float r) :Shape{}, center_{ c }, radius_{ r } {}
Sphere::Sphere(std::string const& s, std::shared_ptr<Material> const& mat, glm::vec3 const& c, float r) : Shape{ s,mat }, center_{ c }, radius_{ r } {}

std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) << "center: " << "(" << center_.x << ", " << center_.y << ", " << center_.z << ")" << " radius: " << radius_;
}

Hitpoint Sphere::intersect(Ray const& r) const {
	if (!Shape::intersect_bounding_box(r)) return Hitpoint{};

	float t;

	Ray obj_ray = {world_to_obj_position(r.origin), world_to_obj_direction(r.direction)};

	bool hit = glm::intersectRaySphere(obj_ray.origin, obj_ray.direction, center_, radius_ * radius_, t);
	if (!hit) return Hitpoint{};

	glm::vec3 point_hit = obj_ray.origin + t * obj_ray.direction;
	glm::vec3 world_hit = obj_to_world_position(point_hit);
	float world_t = glm::distance(world_hit, r.origin);

	return Hitpoint{ hit, world_t, Shape::get_name(), Shape::get_material(), world_hit, r.direction, normal(point_hit) };
}

glm::vec3 Sphere::normal(glm::vec3 const& point) const {
	return obj_to_world_normal(point - center_);
}

void Sphere::create_bounding_box() {
	glm::vec3 min = center_ - glm::vec3(radius_, radius_, radius_);
	glm::vec3 max = center_ + glm::vec3(radius_, radius_, radius_);

	glm::vec3 points[8]; // c-style array should be fine
	points[0] = obj_to_world_position(min);
	points[1] = obj_to_world_position(glm::vec3(min.x, min.y, max.z));
	points[2] = obj_to_world_position(glm::vec3(min.x, max.y, min.z));
	points[3] = obj_to_world_position(glm::vec3(min.x, max.y, max.z));
	points[4] = obj_to_world_position(glm::vec3(max.x, min.y, min.z));
	points[5] = obj_to_world_position(glm::vec3(max.x, min.y, max.z));
	points[6] = obj_to_world_position(glm::vec3(max.x, max.y, min.z));
	points[7] = obj_to_world_position(max);

	min = points[0];
	max = points[7];

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
}


void Sphere::prepare_for_rendering(glm::mat4 const& parent_world_mat) {
	// turn local model matrix into global model matrix
	Shape::update_model_matrix(parent_world_mat);

	// create bounding boxes in global world;
	create_bounding_box();
}