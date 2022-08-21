#include "Shape.hpp"
class Cylinder : public Shape {
private:
	float radius_;
	float height_;
	glm::vec3 bottom_;
public:
	Cylinder(glm::vec3 b, float r, float );
	~Cylinder();

	float area() const override;
	float volume() const override;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& ray) const override;
	glm::vec3 normal(glm::vec3 const& point) const override;
};
