//composit patern
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/vec3.hpp>
#include <cmath>
#include <string>
#include <color.hpp>
class Shape {
protected: //?
	std::string name_{""};
	Color color_{};
public:
	Shape::Shape(Color c, std::string s);
	Shape::Shape();


	virtual float area() const = 0;
	virtual float volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
};
std::ostream& operator<<(std::ostream& os, Shape const& s);
#endif // ! SHAPE_HPP

