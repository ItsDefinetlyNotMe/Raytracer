#include "Box.hpp"
Box::Box(glm::vec3 const& mi, glm::vec3 const& ma):min_{ mi }, max_{ ma }{
	if (min_.x > max_.x) std::swap(min_.x,max_.x);
	if (min_.y > max_.y) std::swap(min_.y, max_.y);
	if (min_.z > max_.z) std::swap(min_.x, max_.x);
}
Box::Box(std::string const& n, Color const& c, glm::vec3 const& mi, glm::vec3 const& ma) : Shape{ n,c }, min_{ mi }, max_{ ma } {}
Box::~Box(){ std::cout << "Destruktor Box" << std::endl; }

float Box::area() const{
	return abs((min_.x - max_.x) * (min_.y - max_.y) * 2) + abs((min_.y - max_.y) * (min_.z - max_.z) * 2) + abs((min_.z - max_.z) * (min_.x - max_.x) * 2);
}

float Box::volume() const{
	return abs((min_.x - max_.x) * (min_.y - max_.y) * (min_.z - max_.z));
}

std::ostream& Box::print(std::ostream& os) const{
	
	return Shape::print(os) << "min: (" << min_.x << "," << min_.y << "," << min_.z << ") " << "max: (" << max_.x << max_.y << max_.z << ")";

}

Hitpoint Box::intersect(Ray const& ray) const{
	return Hitpoint{};
	//ray.origin.x + ray.direction.x * t = min_.x 
}