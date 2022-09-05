#define _USE_MATH_DEFINES
#include "Cylinder.hpp"

Cylinder::Cylinder(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 b, float r, float h) : Shape(n, mat), bottom_(b), radius_(r), height_(h) {}

std::ostream& Cylinder::print(std::ostream& os) const {
	return Shape::print(os) << "position: " << "(" << bottom_.x << ", " << bottom_.y << ", " << bottom_.z << ")" << " radius: " << radius_ << ", height: " << height_;
}

Hitpoint Cylinder::intersect(Ray const& ray) const {
	if (!Shape::intersect_bounding_box(ray)) return Hitpoint{};

	Ray obj_ray = { world_to_obj_position(ray.origin), world_to_obj_direction(ray.direction) };
	
	Hitpoint point{};
	float disb = ((bottom_.y - obj_ray.origin.y) / obj_ray.direction.y);
	glm::vec3 pb = obj_ray.origin + disb * obj_ray.direction;

	float dist = ((bottom_.y + height_ - obj_ray.origin.y) / obj_ray.direction.y);
	glm::vec3 pt = obj_ray.origin + dist * obj_ray.direction;
	

	if (glm::length(pt - glm::vec3{bottom_.x, bottom_.y + height_, bottom_.z}) <= radius_) {
		if (dist >= 0.0f) {
			glm::vec3 world_hit = obj_to_world_position(pt);
			float world_t = glm::distance(world_hit, ray.origin);
			point = Hitpoint{ true, world_t,Shape::get_name(), Shape::get_material(), world_hit, ray.direction, normal(pt) };
		}
	} 

	if (glm::length(pb - bottom_) <= radius_) {
		glm::vec3 world_hit = obj_to_world_position(pb);
		float world_t = glm::distance(world_hit, ray.origin);
		if ((disb < dist && disb >= 0.0f) || dist < 0.0f){
			point = Hitpoint{ true, world_t,Shape::get_name(), Shape::get_material(), world_hit, ray.direction, normal(pb) };
		}
	}
	
	//2d as circle
	glm::vec2 ray_dir_2d{ obj_ray.direction.x,obj_ray.direction.z };
	ray_dir_2d = glm::normalize(ray_dir_2d);
	glm::vec2 U{ (bottom_ - obj_ray.origin).x,(bottom_ - obj_ray.origin).z };
	glm::vec2 U1 = glm::dot(U, ray_dir_2d) * ray_dir_2d;
	glm::vec2 U2{ U - U1 };

	float d = glm::length(U2);
	if (d > radius_)
		return Hitpoint{};

	float m = sqrt(radius_ * radius_ - d * d);
	glm::vec2 p2 = glm::vec2{ obj_ray.origin.x,obj_ray.origin.z } + U1 + m * ray_dir_2d;
	glm::vec2 p1 = glm::vec2{ obj_ray.origin.x,obj_ray.origin.z } + U1 - m * ray_dir_2d;
	//
	float t2 =  (p2.x - obj_ray.origin.x) / obj_ray.direction.x;
	float t1 = (p1.x - obj_ray.origin.x) / obj_ray.direction.x;

	float t = t2;
	if (t1 < t2 && t1 >= 0 || t2 < 0)
		t = t1;

	if (t > 0.0f ) {
		glm::vec3 point_hit = obj_ray.origin + t * obj_ray.direction;
		if (point_hit.y <= bottom_.y + height_ && point_hit.y >= bottom_.y){
			glm::vec3 world_hit = obj_to_world_position(point_hit);
			float world_t = glm::distance(world_hit, ray.origin);
			if(world_t < point.t)
				point = Hitpoint{ true, world_t, Shape::get_name(), Shape::get_material(), world_hit, ray.direction, normal(point_hit) };
		}
	}
	return point;
}


glm::vec3 Cylinder::normal(glm::vec3 const& point) const {
	//oben/unten
	if (floating_equal<float>(point.y, bottom_.y)) {
		return Shape::obj_to_world_normal(glm::vec3{0.0f, -1.0f, 0.0f});
	}
	else if(floating_equal<float>(point.y, bottom_.y + height_)) {
		return Shape::obj_to_world_normal(glm::vec3{ 0.0f, 1.0f, 0.0f });
	}
	//seite
	glm::vec3 normal = glm::vec3{point.x, 0.0f, point.z} - glm::vec3{bottom_.x, 0.0f, bottom_.z};
	return Shape::obj_to_world_normal(normal);
}

void Cylinder::create_bounding_box() {
	// get all 8 transformed points and build bb from there

	glm::vec3 min = bottom_ - glm::vec3(radius_, 0.0f, radius_);
	glm::vec3 max = bottom_ + glm::vec3(radius_, height_, radius_);

	glm::vec3 points[8]; // c-style array should be fine
	points[0] = obj_to_world_position(min);
	points[1] = obj_to_world_position(glm::vec3(min.x, min.y, max.z));
	points[2] = obj_to_world_position(glm::vec3(min.x, max.y, min.z));
	points[3] = obj_to_world_position(glm::vec3(min.x, max.y, max.z));
	points[4] = obj_to_world_position(glm::vec3(max.x, min.y, min.z));
	points[5] = obj_to_world_position(glm::vec3(max.x, min.y, max.z));
	points[6] = obj_to_world_position(glm::vec3(max.x, max.y, min.z));
	points[7] = obj_to_world_position(max);

	min = points[0];
	max = points[1];

	for (int i = 0; i < 8; ++i) {
		if (min.x > points[i].x) min.x = points[i].x;
		if (min.y > points[i].y) min.y = points[i].y;
		if (min.z > points[i].z) min.z = points[i].z;
		if (max.x < points[i].x) max.x = points[i].x;
		if (max.y < points[i].y) max.y = points[i].y;
		if (max.z < points[i].z) max.z = points[i].z;
	}

	Bounding_Box bb {min, max};

	Shape::set_bounding_box(bb);
}

void Cylinder::prepare_for_rendering(glm::mat4 const& parent_world_mat) {
	// turn local model matrix into global model matrix
	Shape::update_model_matrix(parent_world_mat);

	// create bounding boxes in global world;
	create_bounding_box();
}