#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Shape.hpp"
class Triangle : public Shape {
private:
	glm::vec3 left_bottom_;
	glm::vec3 right_bottom_;
	glm::vec3 top_;

public:
	Triangle(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 lb, glm::vec3 rb, glm::vec3 t);

	float area() const override;
	float volume() const override;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& r) const override;
	glm::vec3 normal(glm::vec3 const& point) const override;
	void create_bounding_box() override;
	void prepare_for_rendering(glm::mat4 const& parent_world_mat) override;
};

#endif // TRIANGLE_HPP