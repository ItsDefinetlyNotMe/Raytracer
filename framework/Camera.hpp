#ifndef CAMERA
#define CAMERA
#include "Ray.hpp"
#include "pixel.hpp"
#include <glm/vec3.hpp>
#include <string>

#include <vector>
#include "Shape.hpp"
struct Camera {
	std::string name_;
	float fov_x_;
	glm::vec3 position_{0.0f,0.0f,0.0f};
	glm::vec3 front_{ 0.0f,0.0f,-1.0f };
	glm::vec3 up_{0.0f,1.0f,0.0f};

//	Camera(std::string const& name, float fov_x);
//	Color trace_ray(float x, float y, float aspect, std::vector<std::shared_ptr<Shape>> const& s);
};
#endif // !Camera
