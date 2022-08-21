#include "Shape.hpp"
class Cone : public Shape {
private:
	float radius_;
	float height_;
	glm::vec3 bottom_;
public:
	Cone(glm::vec3 b, float r, float);
	~Cone();

	float area() const override;
	float volume() const override;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& ray) const override;
	glm::vec3 normal(glm::vec3 const& point) const override;
};
