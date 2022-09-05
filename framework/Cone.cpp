#include "Cone.hpp"

Cone::Cone(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 b, float r, float h) : Shape(n, mat), bottom_(b), radius_(r), height_(h) {}

std::ostream& Cone::print(std::ostream& os) const {
	return Shape::print(os) << "position: " << "(" << bottom_.x << ", " << bottom_.y << ", " << bottom_.z << ")" << " radius: " << radius_ << ", height: " << height_;
}

Hitpoint Cone::intersect(Ray const& ray) const {
	if (!Shape::intersect_bounding_box(ray)) return Hitpoint{};

	Ray obj_ray = { world_to_obj_position(ray.origin), world_to_obj_direction(ray.direction) };

	Hitpoint point{};
	float disb = ((bottom_.y - obj_ray.origin.y) / obj_ray.direction.y);//bottom.z = obj_ray-origin.z + disb * obj_ray.direction.z
	glm::vec3 pb = obj_ray.origin + disb * obj_ray.direction;

	if (glm::length(pb - bottom_) <= radius_) {
		if (disb >= 0.0f) {
			glm::vec3 world_hit = obj_to_world_position(pb);
			float world_t = glm::distance(world_hit, ray.origin);
			point =  { true, world_t, Shape::get_name(), Shape::get_material(), world_hit, ray.direction, normal(pb) };
		}
	}
	//
	glm::vec3 h_dach{ glm::vec3{0.0f,bottom_.y - height_, 0.0f} / abs(bottom_.y - height_) };
	glm::vec3 w{ obj_ray.origin - glm::vec3{bottom_.x,bottom_.y + height_,bottom_.z} };
	float m = (radius_*radius_)/(height_*height_) + 1.0f;

	// float a = glm::dot(obj_ray.direction, obj_ray.direction) 
	// 		- m * pow(glm::dot(obj_ray.direction, h_dach), 2) 
	// 		- pow(glm::dot(obj_ray.direction, h_dach), 2);
	float a = glm::dot(obj_ray.direction, obj_ray.direction) 
			- m * obj_ray.direction.y * h_dach.y  * obj_ray.direction.y * h_dach.y;
	float b = 2 * (glm::dot(obj_ray.direction, w) 
				   - m * glm::dot(obj_ray.direction, h_dach) * w.y * h_dach.y);
	float c = glm::dot(w, w) 
			- m * w.y * w.y * h_dach.y * h_dach.y;

	if (b * b - 4 * a * c < 0)
		return Hitpoint{};
	float t1 = (-b + sqrt(b*b -4 * a * c)) / (2 * a);
	float t2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

	if (t1 <= t2 && t1 > 0) {
		glm::vec3 temp = obj_ray.origin + t1 * obj_ray.direction;
		glm::vec3 world_hit = obj_to_world_position(temp);
		float world_t = glm::distance(world_hit, ray.origin);
		if (world_t < point.t) {
			if (temp.y > bottom_.y && temp.y <= bottom_.y + height_)
				point = { true, world_t, Shape::get_name(), Shape::get_material(), world_hit, ray.direction, normal(temp) };
		}
	}
	if (t2 <= t1 && t2 > 0) {
		glm::vec3 temp = obj_ray.origin + t2 * obj_ray.direction;
		glm::vec3 world_hit = obj_to_world_position(temp);
		float world_t = glm::distance(world_hit, ray.origin);
		if (world_t < point.t) {
			if (temp.y > bottom_.y && temp.y <= bottom_.y + height_)
				point = { true, world_t, Shape::get_name(), Shape::get_material(), world_hit, ray.direction, normal(temp) };
		}
	}
	return point;
}

glm::vec3 Cone::normal(glm::vec3 const& point) const {
	if (floating_equal<float>(point.y, bottom_.y))
		return obj_to_world_normal({ 0.0f, -1.0f, 0.0f });
	glm::vec3 norm{ point - bottom_ };
	norm.y = 0.0f;
	norm = glm::normalize(norm);
	norm.x = norm.x * (height_ / radius_);
	norm.z = norm.z * (height_ / radius_);
	norm.y = radius_ / height_;
	return obj_to_world_normal(norm);
}

void Cone::create_bounding_box() {
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
	max = points[7];

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

void Cone::prepare_for_rendering(glm::mat4 const& parent_world_mat) {
	// turn local model matrix into global model matrix
	Shape::update_model_matrix(parent_world_mat);

	// create bounding boxes in global world;
	create_bounding_box();
}