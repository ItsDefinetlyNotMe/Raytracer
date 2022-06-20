//composit patern
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/vec3.hpp>
#include <cmath>
#include <string>
#include <color.hpp>
class Shape {
private:
	std::string name_;
	Color color_;
public:
	virtual float area() = 0;
	virtual float volume() = 0;
	virtual std::ostream& print(std::ostream& os) const;
};
std::ostream& operator<<(std::ostream& os, Shape const& s);
#endif // ! SHAPE_HPP

