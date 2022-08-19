#include "Box.hpp"
#include "helper.hpp"
#include <glm/glm.hpp>
Box::Box(glm::vec3 const& mi, glm::vec3 const& ma) :min_{ mi }, max_{ ma }{
	if (min_.x > max_.x) std::swap(min_.x, max_.x);
	if (min_.y > max_.y) std::swap(min_.y, max_.y);
	if (min_.z > max_.z) std::swap(min_.x, max_.x);
}

Box::Box(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 const& mi, glm::vec3 const& ma) : Shape{ n,mat }, min_{ mi }, max_{ ma } {}

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
	
	// if (!Shape::intersect_bounding_box(ray)) return Hitpoint{};

	Ray obj_ray = {world_to_obj_position(ray.origin), glm::normalize(world_to_obj_direction(ray.direction))};
	
	//vlt eleganter ?
	bool not_inside = true;//true = r�ckseite sehen
	if (obj_ray.origin.x > min_.x && obj_ray.origin.x < max_.x && obj_ray.origin.y > min_.y && obj_ray.origin.y < max_.y && obj_ray.origin.z > min_.z && obj_ray.origin.z < max_.z) {
		not_inside = false;
	}
	if (obj_ray.direction.x < 0 != not_inside) {
		float t = (min_.x - obj_ray.origin.x) / obj_ray.direction.x;
		glm::vec3 temp{ obj_ray.origin + obj_ray.direction * t };
		float world_t = t * Shape::get_scale();
		glm::vec3 world_temp { ray.origin + ray.direction * world_t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
			return  Hitpoint{ true, world_t,Shape::get_name(),Shape::get_material(), world_temp,ray.direction };
		}
	}
	else if (obj_ray.direction.x > 0 != not_inside) {
		float t = (max_.x - obj_ray.origin.x) / obj_ray.direction.x;
		glm::vec3 temp{ obj_ray.origin + obj_ray.direction * t };
		float world_t = t * Shape::get_scale();
		glm::vec3 world_temp { ray.origin + ray.direction * world_t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.y > min_.y && temp.y < max_.y) && t >= 0) {
			return  Hitpoint{ true, world_t,Shape::get_name(),Shape::get_material(), world_temp,ray.direction };
		}
	}

	if (obj_ray.direction.y < 0 != not_inside) {
		float t = (min_.y - obj_ray.origin.y) / obj_ray.direction.y;
		glm::vec3 temp{ obj_ray.origin + obj_ray.direction * t };
		float world_t = t * Shape::get_scale();
		glm::vec3 world_temp { ray.origin + ray.direction * world_t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true, world_t,Shape::get_name(),Shape::get_material(), world_temp,ray.direction };
		}
	}
	else if (obj_ray.direction.y > 0 != not_inside) {
		float t = (max_.y - obj_ray.origin.y) / obj_ray.direction.y;
		glm::vec3 temp{ obj_ray.origin + obj_ray.direction * t };
		float world_t = t * Shape::get_scale();
		glm::vec3 world_temp { ray.origin + ray.direction * world_t };
		if ((temp.z > min_.z && temp.z < max_.z) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true, world_t,Shape::get_name(),Shape::get_material(), world_temp,ray.direction };
		}
	}

	if (obj_ray.direction.z < 0 != not_inside) {
		float t = (min_.z - obj_ray.origin.z) / obj_ray.direction.z;
		glm::vec3 temp{ obj_ray.origin + obj_ray.direction * t };
		float world_t = t * Shape::get_scale();
		glm::vec3 world_temp { ray.origin + ray.direction * world_t };
		if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true, world_t,Shape::get_name(),Shape::get_material(), world_temp,ray.direction };
		}
	}
	else if (obj_ray.direction.z > 0 != not_inside) {
		float t = (max_.z - obj_ray.origin.z) / obj_ray.direction.z;
		glm::vec3 temp{ obj_ray.origin + obj_ray.direction * t };
		float world_t = t * Shape::get_scale();
		glm::vec3 world_temp { ray.origin + ray.direction * world_t };
		if ((temp.y > min_.y && temp.y < max_.y) && (temp.x > min_.x && temp.x < max_.x) && t >= 0) {
			return  Hitpoint{ true, world_t,Shape::get_name(),Shape::get_material(), world_temp,ray.direction };
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

	glm::vec3 obj_point = Shape::world_to_obj_position(point);

	if (floating_equal<float>(obj_point.x, min_.x))
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3 {-1,  0,  0}));

	else if (floating_equal<float>(obj_point.x,max_.x))
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3 { 1,  0,  0}));

	else if (floating_equal<float>(obj_point.y,min_.y))
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3 { 0, -1,  0}));

	else if (floating_equal<float>(obj_point.y,max_.y))
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3 { 0,  1,  0}));

	else if (floating_equal<float>(obj_point.z,min_.z))
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3 { 0,  0, -1}));

	else if (floating_equal<float>(obj_point.z,max_.z))
		return glm::normalize(Shape::obj_to_world_direction(glm::vec3 { 0,  0,  1}));

	return glm::vec3{};
}

Bounding_Box Box::create_bounding_box() {
	// get all 8 transformed points and build bb from there
	glm::vec3 points[8]; // c-style array should be fine
	points[0] = obj_to_world_position(min_);
	points[1] = obj_to_world_position(glm::vec3(min_.x, min_.y, max_.z));
	points[2] = obj_to_world_position(glm::vec3(min_.x, max_.y, min_.z));
	points[3] = obj_to_world_position(glm::vec3(min_.x, max_.y, max_.z));
	points[4] = obj_to_world_position(glm::vec3(max_.x, min_.y, min_.z));
	points[5] = obj_to_world_position(glm::vec3(max_.x, min_.y, max_.z));
	points[6] = obj_to_world_position(glm::vec3(max_.x, max_.y, min_.z));
	points[7] = obj_to_world_position(max_);

	glm::vec3 min = points[0];
	glm::vec3 max = points[1];
	for (int i = 2; i < 8; ++i) {
		if (min.x > points[i].x) min.x = points[i].x;
		if (min.y > points[i].y) min.x = points[i].y;
		if (min.z > points[i].z) min.x = points[i].z;
		if (max.x < points[i].x) max.x = points[i].x;
		if (max.y < points[i].y) max.x = points[i].y;
		if (max.z < points[i].z) max.x = points[i].z;
	}

	Bounding_Box bb {min, max};

	Shape::set_bounding_box(bb);
	return bb;
}