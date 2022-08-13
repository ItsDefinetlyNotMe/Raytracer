#include "Shape.hpp"
#include "color.hpp"
class Box :public Shape {
private:
	glm::vec3 min_;
	glm::vec3 max_;
public:
	Box(glm::vec3 const& mi, glm::vec3 const& ma);
	Box(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 const& mi, glm::vec3 const& ma);
	~Box();


	float area() const;
	float volume() const;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& ray) const override;
	glm::vec3 normal(glm::vec3 const& point) const override;
};
//std::ostream& operator<<(std::ostream& os, Box const& b);
