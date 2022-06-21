#include "Shape.hpp"
Shape::Shape(Color c, std::string s):color_{ c }, name_{ s }{};
Shape::Shape(){};

std::ostream& Shape::print(std::ostream& os) const{
	os << "Name: " << name_ << " Color: " << color_;
	return os;
}
std::ostream& operator<<(std::ostream& os, Shape const& s) {
	s.print(os);
	return os;
}