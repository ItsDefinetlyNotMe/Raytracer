#ifndef HITPOINT_HPP
#define HITPOINT_HPP

#include <string>
#include <math.h>

#include <glm/vec3.hpp>
#include "Material.hpp"
struct Hitpoint {
	bool hit{ false };
	float t = INFINITY; 
	std::string name{ "" };
	std::shared_ptr<Material> mat{};
	glm::vec3 point3d{ 0.0f,0.0f,0.0f };
	glm::vec3 direction{ 0.0f,0.0f,-1.0f };
	glm::vec3 normal{0.0f, 0.0f, -1.0f};
};
#endif // !HITPOINT_HPP

