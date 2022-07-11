#include "Shape.hpp"
// Shape::Shape(std::string const& s, Color const& c): name_{ s }, color_{ c }{};

Shape::Shape(std::string const& s, std::shared_ptr<Material> const& m): name_{ s }, mat_{ m } {};

Shape::Shape(){};

Shape::~Shape() {
	std::cout << "Destruktor Shape" << std::endl;
};

// Color Shape::get_color() const {
// 	return color_;
// }

std::shared_ptr<Material> Shape::get_material() const {
	return mat_;
}

std::string Shape::get_name() const {
	return name_;
}


std::ostream& Shape::print(std::ostream& os) const{
	return os << "Name: " << name_ << ", Material: " << mat_ << "\n";
}
std::ostream& operator<<(std::ostream& os, Shape const& s) {
	return s.print(os);
}