#include "Shape.hpp"
class Triangle : public Shape {
private:
	glm::vec3 left_bottom_;
	glm::vec3 right_bottom_;
	glm::vec3 top_;

public:
	Triangle(glm::vec3 lb, glm::vec3 rb, glm::vec3 t);
	//~Triangle();

	float area() const override;
	float volume() const override;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& r) const override;
	glm::vec3 normal(glm::vec3 const& point) const override;
};
