//composit patern
#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "Hitpoint.hpp"
#include "Ray.hpp"
#include <glm/vec3.hpp>
#include <cmath>
#include <string>
#include "color.hpp"
class Shape {
private: 
	std::string name_{""};
	Color color_{};
public:
	Shape(std::string const& s, Color const& c);
	Shape();
	virtual ~Shape();


	virtual float area() const = 0;
	virtual float volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
	Color get_color() const;
	std::string get_name() const;
	virtual Hitpoint intersect(Ray const& ray) const = 0;
};
std::ostream& operator<<(std::ostream& os, Shape const& s);
#endif // ! SHAPE_HPP

