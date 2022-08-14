#include "Box.hpp"
#include "helper.hpp"
#include <glm/glm.hpp>
Box::Box(glm::vec3 const& mi, glm::vec3 const& ma) :min_{ mi }, max_{ ma }{
	if (min_.x > max_.x) std::swap(min_.x, max_.x);
	if (min_.y > max_.y) std::swap(min_.y, max_.y);
	if (min_.z > max_.z) std::swap(min_.x, max_.x);
}

Box::Box(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 const& mi, glm::vec3 const& ma) : Shape{ n,mat }, min_{ mi }, max_{ ma } {}

Box::~Box() { std::cout << "Destruktor Box" << std::endl; }

float Box::area() const {
	return abs((min_.x - max_.x) * (min_.y - max_.y) * 2) + abs((min_.y - max_.y) * (min_.z - max_.z) * 2) + abs((min_.z - max_.z) * (min_.x - max_.x) * 2);
}

float Box::volume() const {
	return abs((min_.x - max_.x) * (min_.y - max_.y) * (min_.z - max_.z));
}

std::ostream& Box::print(std::ostream& os) const {

	return Shape::print(os) << "min: (" << min_.x << "," << min_.y << "," << min_.z << ") " << "max: (" << max_.x << "," << max_.y << "," << max_.z << ")";

}
//inside swapped with not_inside
Hitpoint Box::intersect(Ray const& ray) const {
	
	//vlt eleganter ?
	bool not_inside = true;//true = rückseite sehen
	if (ray.origin.x > min_.x && ray.origin.x < max_.x && ray.origin.y > min_.y && ray.origin.y < max_.y && ray.origin.z > min_.z && ray.origin.z < max_.z) {
		not_inside = false;
	}
	if (ray.direction.x < 0 != not_inside) {
		float t = (min_.x - ray.origin.x) / ray.direction.x;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
		}
	}
	else if (ray.direction.x > 0 != not_inside) {
		float t = (max_.x - ray.origin.x) / ray.direction.x;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
		}
	}

	if (ray.direction.y < 0 != not_inside) {
		float t = (min_.y - ray.origin.y) / ray.direction.y;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
		}
	}
	else if (ray.direction.y > 0 != not_inside) {
		float t = (max_.y - ray.origin.y) / ray.direction.y;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
		}
	}

	if (ray.direction.z < 0 != not_inside) {
		float t = (min_.z - ray.origin.z) / ray.direction.z;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
		}
	}
	else if (ray.direction.z > 0 != not_inside) {
		float t = (max_.z - ray.origin.z) / ray.direction.z;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
		}
	}
	/*
	float t = (min_.x - ray.origin.x) / ray.direction.x;
	glm::vec3 temp{ ray.origin + ray.direction * t };
	if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
		return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
	}
	
	t = (max_.x - ray.origin.x) / ray.direction.x;
	temp =  ray.origin + ray.direction * t ;
	if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
		return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
	}

	t = (min_.y - ray.origin.y) / ray.direction.y;
	temp = ray.origin + ray.direction * t ;
	if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
		return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
	}

	t = (max_.y - ray.origin.y) / ray.direction.y;
	temp = ray.origin + ray.direction * t ;
	if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
		return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
	}

	t = (min_.z - ray.origin.z) / ray.direction.z;
	temp = ray.origin + ray.direction * t;
	if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
		return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
	}

	t = (max_.z - ray.origin.z) / ray.direction.z;
	temp =  ray.origin + ray.direction * t;
	if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
		return  Hitpoint{ true,t,Shape::get_name(),Shape::get_material(),temp,ray.direction };
	}*/
	return Hitpoint{};
}

glm::vec3 Box::normal(glm::vec3 const& point) const {
	//!!Assumption point is on the box surface
	//! FLOATS
	if (floating_equal<float>(point.x, min_.x))
		return -glm::normalize(glm::vec3{ max_.x,min_.y,min_.z } - min_);

	else if (floating_equal<float>(point.x,max_.x))
		return -glm::normalize(min_ - glm::vec3{ max_.x,min_.y,min_.z });

	else if (floating_equal<float>(point.y,min_.y))
		return -glm::normalize(glm::vec3{ min_.x,max_.y,min_.z } - min_);

	else if (floating_equal<float>(point.y,max_.y))
		return -glm::normalize(min_ - glm::vec3{ min_.x,max_.y,min_.z });

	else if (floating_equal<float>(point.z,min_.z))
		return -glm::normalize(glm::vec3{ min_.x,min_.y,max_.z } - min_);

	else if (floating_equal<float>(point.z,max_.z))
		return -glm::normalize(min_ - glm::vec3{ min_.x,min_.y,max_.z });

	return glm::vec3{};
}
