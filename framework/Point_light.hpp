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
};
#endif // !POINT_LIGHT
