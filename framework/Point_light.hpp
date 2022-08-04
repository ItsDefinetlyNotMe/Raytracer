#ifndef POINT_LIGHT
#define POINT_LIGHT
#include <glm/vec3.hpp>
#include "color.hpp"
class Point_Light {
	// ist ein struct hier besser angebracht ?
private:
	std::string name_;
	glm::vec3 position_;
	Color color_;
	glm::vec3 brightness_;
public:
	Point_Light(std::string const& n, glm::vec3 const& p,Color const& c,glm::vec3 const& b);
};
#endif // !POINT_LIGHT
