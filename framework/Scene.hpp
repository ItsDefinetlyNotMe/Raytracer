#ifndef SCENE
#define SCENE
#include <vector> 
#include "Shape.hpp"
#include "Camera.hpp" 
#include "Point_light.hpp"
#include "Area_light.hpp"

struct Scene {
	// std::vector<std::shared_ptr<Shape>> world_{ nullptr };//composite muss noch gabut werden
	std::shared_ptr<Shape> root_{nullptr};
	std::vector<std::shared_ptr<Camera>> cameras_{ nullptr };
	std::vector<std::shared_ptr<Point_Light>>lights_{ nullptr };
	std::vector<std::shared_ptr<Area_Light>>a_lights_{ nullptr };

	Color ambient_{0.2f,0.2f,0.2f};
};
#endif // !SCENE