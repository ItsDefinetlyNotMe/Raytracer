#ifndef SCENE
#define SCENE


#import <vector> 
#import "Shape.hpp"
#import "Camera.hpp" 
#import "renderer.hpp"
#import "Point_light.hpp"

struct Scene{
	std::vector<std::shared_ptr<Shape>> world_;//composite muss noch gabut werden
	std::vector<std::shared_ptr<Camera>> cameras_;
	std::vector<std::shared_ptr<Point_Light>>lights_{};
	//ambient_;
	std::shared_ptr<Renderer> render_;
};
#endif // !SCENE