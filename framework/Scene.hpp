#ifndef SCENE
#define SCENE
#include <vector> 
#include "Shape.hpp"
#include "Camera.hpp" 
#include "renderer.hpp"
#include "Point_light.hpp"

struct Scene {
	std::vector<std::shared_ptr<Shape>> world_{ nullptr };//composite muss noch gabut werden
	std::vector<std::shared_ptr<Camera>> cameras_{ nullptr };
	std::vector<std::shared_ptr<Point_Light>>lights_{ nullptr };
	//ambient_;
	std::shared_ptr<Renderer> render_{ nullptr };
};
#endif // !SCENE