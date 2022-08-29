#ifndef CONE_HPP
#define CONE_HPP

#include "Shape.hpp"
class Cone : public Shape {
private:
	float radius_;
	float height_;
	glm::vec3 bottom_;
public:
	Cone(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 b, float r, float h);

	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& ray) const override;
	glm::vec3 normal(glm::vec3 const& point) const override;
	Bounding_Box create_bounding_box() override;
};

#endif // CONE_HPP