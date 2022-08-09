#ifndef CAMERA
#define CAMERA
#include "Ray.hpp"

#include <glm/vec3.hpp>
#include <string>
class Camera {
private:
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	std::string name_;
	float fov_x_;
public:
	Camera(std::string const& name, float fov_x);
	Color trace_ray(Pixel const& p, float aspect,Scene const& s);
};
#endif // !Camera
