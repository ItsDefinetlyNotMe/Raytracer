#include "Shape.hpp"
#include "Hitpoint.hpp"
#include "Ray.hpp"
class Sphere : Shape {
private:
	glm::vec3 center_;
	float radius_;

	
public:
	Sphere(float r);
	Sphere(glm::vec3 const& c);
	Sphere(glm::vec3 const& c, float r );
	Sphere(glm::vec3 const& c, float r, Color co, std::string s);


	float area() const override;
	float volume() const override;
	std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& r) const;
};
