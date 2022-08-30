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

	float area() const override;
	float volume() const override;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& r) const override;
	glm::vec3 normal(glm::vec3 const& point) const override;
	void create_bounding_box() override;
	void prepare_for_rendering(glm::mat4 const& parent_world_mat) override;
};

//std::ostream& operator<<(std::ostream& os, Sphere const& s);
#endif // !SPHERE_HPP
