//composit patern
#include <glm.hpp>
virtual class Shape {
	virtual double area();
	virtual double volume();
};
class Sphere : Shape {
private:	
	glm::vec3 mid_;
public:
};
class Box : Shape {

};