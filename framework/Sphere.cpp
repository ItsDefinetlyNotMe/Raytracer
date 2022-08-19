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
	// if (!Shape::intersect_bounding_box(r)) return Hitpoint{};

	float t;

	Ray obj_ray = {world_to_obj_position(r.origin), glm::normalize(world_to_obj_direction(r.direction))};

	bool hit = glm::intersectRaySphere(obj_ray.origin, obj_ray.direction, center_, radius_ * radius_, t);

	float world_t = t * Shape::get_scale();

	return Hitpoint{ hit, world_t,Shape::get_name(),Shape::get_material(),r.origin + r.direction * world_t,r.direction };
}

glm::vec3 Sphere::normal(glm::vec3 const& point) const {
	return glm::normalize(point - Shape::obj_to_world_position(center_));
}

Bounding_Box Sphere::create_bounding_box() {
	Bounding_Box bb;

	bb.min_ = glm::vec3(center_.x - radius_ * Shape::get_scale(),
						center_.y - radius_ * Shape::get_scale(),
						center_.z - radius_ * Shape::get_scale());
	bb.max_ = glm::vec3(center_.x + radius_ * Shape::get_scale(),
						center_.y + radius_ * Shape::get_scale(),
						center_.z + radius_ * Shape::get_scale());
	
	Shape::set_bounding_box(bb);
	return bb;
}