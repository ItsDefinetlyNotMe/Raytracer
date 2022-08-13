#define _USE_MATH_DEFINES
#include "Sphere.hpp"
#include <glm/gtx/intersect.hpp>

Sphere::Sphere(glm::vec3 const& c, float r) :Shape{}, center_{ c }, radius_{ r } {}
Sphere::Sphere(std::string const& s, std::shared_ptr<Material> const& mat, glm::vec3 const& c, float r) : Shape{ s,mat }, center_{ c }, radius_{ r } {}
Sphere::~Sphere() { std::cout << "Destruktor Sphere" << std::endl; };

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
	float t;
	bool hit = glm::intersectRaySphere(r.origin, glm::normalize(r.direction), center_, radius_ * radius_, t);
	return Hitpoint{ hit,t,Shape::get_name(),Shape::get_material(),r.origin + r.direction * t,r.direction };
}

glm::vec3 Sphere::normal(glm::vec3 const& point) const {
	return glm::normalize(point - center_);
}

