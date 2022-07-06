#include "Box.hpp"
Box::Box(glm::vec3 const& mi, glm::vec3 const& ma):min_{ mi }, max_{ ma }{
	if (min_.x > max_.x) std::swap(min_.x,max_.x);
	if (min_.y > max_.y) std::swap(min_.y, max_.y);
	if (min_.z > max_.z) std::swap(min_.x, max_.x);
}
Box::Box(std::string const& n, Color const& c, glm::vec3 const& mi, glm::vec3 const& ma) : Shape{ n,c }, min_{ mi }, max_{ ma } {}
Box::~Box(){ std::cout << "Destruktor Box" << std::endl; }

float Box::area() const{
	return abs((min_.x - max_.x) * (min_.y - max_.y) * 2) + abs((min_.y - max_.y) * (min_.z - max_.z) * 2) + abs((min_.z - max_.z) * (min_.x - max_.x) * 2);
}

float Box::volume() const{
	return abs((min_.x - max_.x) * (min_.y - max_.y) * (min_.z - max_.z));
}

std::ostream& Box::print(std::ostream& os) const{
	
	return Shape::print(os) << "min: (" << min_.x << "," << min_.y << "," << min_.z << ") " << "max: (" << max_.x << max_.y << max_.z << ")";

}

Hitpoint Box::intersect(Ray const& ray) const{
	//vlt eleganter ?
	bool inside = false;
	if (ray.origin.x > min_.x && ray.origin.x < max_.x && ray.origin.y > min_.y && ray.origin.y < max_.y && ray.origin.z > min_.z && ray.origin.z < max_.z) {
		inside = true;
	}
	if (ray.direction.x < 0 != inside) {
		float t = (min_.x - ray.origin.x) / ray.direction.x;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_color(),temp,ray.direction };
		}
	}
	else if (ray.direction.x > 0 != inside) {
		float t = (max_.x - ray.origin.x) / ray.direction.x;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_color(),temp,ray.direction };
		}

		if (ray.direction.y < 0 != inside) {
			float t = (min_.y - ray.origin.y) / ray.direction.y;
			glm::vec3 temp{ ray.origin + ray.direction * t };
			if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
				return  Hitpoint{ true,t,Shape::get_name(),Shape::get_color(),temp,ray.direction };
			}
		}
		else if (ray.direction.y > 0 != inside) {
			float t = (max_.y - ray.origin.y) / ray.direction.y;
			glm::vec3 temp{ ray.origin + ray.direction * t };
			if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
				return  Hitpoint{ true,t,Shape::get_name(),Shape::get_color(),temp,ray.direction };
			}
		}

		if (ray.direction.z < 0 != inside) {
			float t = (min_.z - ray.origin.z) / ray.direction.z;
			glm::vec3 temp{ ray.origin + ray.direction * t };
			if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
				return  Hitpoint{ true,t,Shape::get_name(),Shape::get_color(),temp,ray.direction };
			}
		}
		else if (ray.direction.z > 0 != inside) {
			float t = (max_.z - ray.origin.z) / ray.direction.z;
			glm::vec3 temp{ ray.origin + ray.direction * t };
			if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
				return  Hitpoint{ true,t,Shape::get_name(),Shape::get_color(),temp,ray.direction };
			}
		}
}