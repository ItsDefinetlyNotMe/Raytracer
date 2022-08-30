#define _USE_MATH_DEFINES
#include "Sphere.hpp"
#include <glm/gtx/intersect.hpp>

Sphere::Sphere(glm::vec3 const& c, float r) :Shape{}, center_{ c }, radius_{ r } {}
Sphere::Sphere(std::string const& s, std::shared_ptr<Material> const& mat, glm::vec3 const& c, float r) : Shape{ s,mat }, center_{ c }, radius_{ r } {}

float Sphere::area() const {
	return M_PI * 4 * pow(radius_, 2);
}

float Sphere::volume() const {
	return (4.0f / 3.0f) * M_PI * pow(abs(radius_), 3);
}

std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) << "center: " << "(" << center_.x << ", " << center_.y << ", " << center_.z << ")" << " radius: " << radius_;
}

Hitpoint Sphere::intersect(Ray const& r) const {
	if (!Shape::intersect_bounding_box(r)) return Hitpoint{};

	float t;

	Ray obj_ray = {world_to_obj_position(r.origin), glm::normalize(world_to_obj_direction(r.direction))};

	bool hit = glm::intersectRaySphere(obj_ray.origin, obj_ray.direction, center_, radius_ * radius_, t);

	float world_t = t * Shape::get_scale();

	return Hitpoint{ hit, world_t,Shape::get_name(),Shape::get_material(),r.origin + r.direction * world_t, r.direction, normal(r.origin + r.direction * world_t) };
}

glm::vec3 Sphere::normal(glm::vec3 const& point) const {
	return glm::normalize(point - Shape::obj_to_world_position(center_));
}

void Sphere::create_bounding_box() {
	Bounding_Box bb;

	glm::vec3 w_center = obj_to_world_position(center_);
	float scale = Shape::get_scale();

	bb.min_ = glm::vec3(w_center.x - radius_ * scale,
						w_center.y - radius_ * scale,
						w_center.z - radius_ * scale);
	bb.max_ = glm::vec3(w_center.x + radius_ * scale,
						w_center.y + radius_ * scale,
						w_center.z + radius_ * scale);
	
	Shape::set_bounding_box(bb);
}


void Sphere::prepare_for_rendering(glm::mat4 const& parent_world_mat) {
	// turn local model matrix into global model matrix
	Shape::update_model_matrix(parent_world_mat);

	// create bounding boxes in global world;
	create_bounding_box();
}