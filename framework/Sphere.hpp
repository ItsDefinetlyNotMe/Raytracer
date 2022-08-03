#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Shape.hpp"
#include "Hitpoint.hpp"
#include "Ray.hpp"
class Sphere : public Shape {
private:
	glm::vec3 center_;
	float radius_;
public:
	Sphere(float r);
	Sphere(glm::vec3 const& c);
	Sphere(glm::vec3 const& c, float r);
	Sphere(std::string const& s, std::shared_ptr<Material> const& mat, glm::vec3 const& c, float r);
	~Sphere();

	float area() const override;
	float volume() const override;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& r) const override;
};

//std::ostream& operator<<(std::ostream& os, Sphere const& s);
#endif // !SPHERE_HPP
