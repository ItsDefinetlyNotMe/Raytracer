#ifndef CAMERA
#define CAMERA
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
	Camera(std::string name, float fov_x);
};
#endif // !Camera
