#ifndef POINT_LIGHT
#define POINT_LIGHT
#include <glm/vec3.hpp>
#include "color.hpp"
struct Point_Light {
	std::string name_;
	glm::vec3 position_;
	Color color_;
	float brightness_;
};
#endif // !POINT_LIGHT
