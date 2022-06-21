#define _USE_MATH_DEFINES
#include "Sphere.hpp"
#include <glm/gtx/intersect.hpp>
Sphere::Sphere(glm::vec3 const& c):Shape{},center_ { c } {}
Sphere::Sphere(glm::vec3 const& c, float r):Shape{}, center_ { c }, radius_{ r } {}//konstruktoren 
Sphere::Sphere(glm::vec3 const& c, float r,Color co,std::string s):Shape(co,s), center_{ c }, radius_{ r } {}//konstruktoren 

float Sphere::area() const{
	return M_PI * 4 * pow(radius_,2);
}
float Sphere::volume() const{
	//4/3 · pi · r3
	return 4.0f / 3.0f * M_PI * pow(radius_,3);
}
std::ostream& Sphere::print(std::ostream& os) const{
	Shape::print;
	return os << "center: " << "(" << center_.x << ", " << center_.y << ", " << center_.z << ")" << " radius: " << radius_;
}
Hitpoint Sphere::intersect(Ray const& r) const {
	float t;
	bool hit = glm::intersectRaySphere(r.origin , glm::normalize(r.direction),center_,radius_*radius_,t);
	return Hitpoint{ hit,t,name_,color_,r.origin + r.direction * t,r.direction };
}