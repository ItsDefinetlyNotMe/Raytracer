#define _USE_MATH_DEFINES
#include "Sphere.hpp"
Sphere::Sphere(glm::vec3 const& c):center_{c} {}
Sphere::Sphere(glm::vec3 const& c, float r): center_{ c }, radius_{ r } {}
float Sphere::area() const{
	return M_PI * 4 * pow(radius_,2);
}
float Sphere::volume() const{
	//4/3 · pi · r3
	return 4.0f / 3.0f * M_PI * pow(radius_,3);
}