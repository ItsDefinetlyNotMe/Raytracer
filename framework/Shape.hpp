//composit patern
#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "Hitpoint.hpp"
#include "Ray.hpp"
#include <glm/vec3.hpp>
#include <cmath>
#include <string>
#include <memory>
#include "Material.hpp"
class Shape {
private:
	std::string name_{ "" };
	std::shared_ptr<Material> mat_;
public:
	// Shape(std::string const& s, Color const& c);
	Shape(std::string const& s, std::shared_ptr<Material> const& m);
	Shape();
	virtual ~Shape();


	virtual float area() const = 0;
	virtual float volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
	// Color get_color() const;
	std::shared_ptr<Material> get_material() const;
	std::string get_name() const;
	virtual Hitpoint intersect(Ray const& ray) const = 0;
	virtual glm::vec3 normal(glm::vec3 const& point) const = 0;
};
std::ostream& operator<<(std::ostream& os, Shape const& s);
#endif // ! SHAPE_HPP
