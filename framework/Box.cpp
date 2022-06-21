#include "Box.hpp"
Box::Box(glm::vec3 const& mi, glm::vec3 const& ma):min_{ mi }, max_{ ma }{}

float Box::area() const{
	return abs((min_.x - max_.x) * (min_.y - max_.y) * 2) + abs((min_.y - max_.y) * (min_.z - max_.z) * 2) + abs((min_.z - max_.z) * (min_.x - max_.x) * 2);
}

float Box::volume() const{
	return abs((min_.x - max_.x) * (min_.y - max_.y) * (min_.z - max_.z));
}
std::ostream& Box::print(std::ostream& os) const{
	Shape::print;
	return os << "min: (" << min_.x << min_.y << min_.z << ") "  << "max: (" << max_.x << max_.y << max_.z << ")";

}