#include "Shape.hpp"
class Box : Shape {
private:
	glm::vec3 min_;
	glm::vec3 max_;
public:
	Box(glm::vec3 const& mi,glm::vec3 const& ma);

	float area() const;
	float volume() const;
	std::ostream& print(std::ostream& os) const override;
};