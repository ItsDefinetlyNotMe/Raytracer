#include "Shape.hpp"
class Sphere : Shape {
private:
	glm::vec3 center_;
	float radius_;
public:
	Sphere(float r);
	Sphere(glm::vec3 const& c);
	Sphere(glm::vec3 const& c, float r );

	float area() const;
	float volume() const;
	std::ostream& print(std::ostream& os) const override;

};
