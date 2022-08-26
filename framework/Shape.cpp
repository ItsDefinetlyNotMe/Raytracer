#include "Shape.hpp"
Shape::Shape(std::string const& s): name_{ s } {};

Shape::Shape(std::string const& s, std::shared_ptr<Material> const& m) : name_{ s }, mat_{ m } {};

Shape::Shape() {};

std::shared_ptr<Material> Shape::get_material() const {
	return mat_;
}

std::string Shape::get_name() const {
	return name_;
}

glm::mat4 Shape::get_model_matrix() const {
	return model_;
}

float Shape::get_scale() const {
	return scale_.scale;
}

Bounding_Box Shape::get_bounding_box() const {
	return bounding_box_;
}

void Shape::set_translation(Translation const& translation) {
	translation_ = translation;
	update_model_matrix();
}

void Shape::set_rotation(Rotation const& rot) {
	rotations_.push_back(rot);
	update_model_matrix();
}

void Shape::set_scaling(Scaling const& scale) {
	scale_ = scale;
	update_model_matrix();
}

void Shape::set_bounding_box(Bounding_Box const& bounding_box) {
	bounding_box_ = bounding_box;
}

void Shape::update_model_matrix() {
	glm::mat4 translation_mat = glm::translate(glm::mat4(), translation_.translate);
	
	glm::mat4 rotation_mat = glm::mat4(1.0f);
	// may need to iterate in reverse order not sure
	// for (auto const& rotation : rotations_) {
	// 	rotation_mat = glm::rotate(rotation.angle, rotation.vector) * rotation_mat;
	// }

	for (auto it = rotations_.rbegin(); it != rotations_.rend(); ++it) {
		rotation_mat = glm::rotate(it->angle, it->vector) * rotation_mat;
	}

	// uniform scaling for now
	glm::mat4 scale_mat = glm::scale(glm::vec3(scale_.scale));

	model_ = translation_mat * rotation_mat * scale_mat;
}

glm::vec3 Shape::obj_to_world_position(glm::vec3 const& position) const{
	return glm::vec3(model_ * glm::vec4(position, 1.0f));
}

glm::vec3 Shape::obj_to_world_direction(glm::vec3 const& direction) const{
	return glm::normalize(glm::vec3(model_ * glm::vec4(direction, 0.0f)));
}

glm::vec3 Shape::world_to_obj_position(glm::vec3 const& position) const{
	return glm::vec3(glm::inverse(model_) * glm::vec4(position, 1.0f));
}

glm::vec3 Shape::world_to_obj_direction(glm::vec3 const& direction) const{
	return glm::normalize(glm::vec3(glm::inverse(model_) * glm::vec4(direction, 0.0f)));
}

bool Shape::intersect_bounding_box(Ray const& ray) const{

	// bb is axis aligned in current space
	bool not_inside = true;//true = r�ckseite sehen
	if (ray.origin.x > bounding_box_.min_.x && ray.origin.x < bounding_box_.max_.x && ray.origin.y > bounding_box_.min_.y && ray.origin.y < bounding_box_.max_.y && ray.origin.z > bounding_box_.min_.z && ray.origin.z < bounding_box_.max_.z) {
		not_inside = false;
	}

	if (ray.direction.x < 0 != not_inside) {
		float t = (bounding_box_.min_.x - ray.origin.x) / ray.direction.x;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > bounding_box_.min_.z && temp.z < bounding_box_.max_.z) && (temp.y > bounding_box_.min_.y && temp.y < bounding_box_.max_.y) && t >= 0) {
			return  true;
		}
	}
	else if (ray.direction.x > 0 != not_inside) {
		float t = (bounding_box_.max_.x - ray.origin.x) / ray.direction.x;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > bounding_box_.min_.z && temp.z < bounding_box_.max_.z) && (temp.y > bounding_box_.min_.y && temp.y < bounding_box_.max_.y) && t >= 0) {
			return  true;
		}
	}

	if (ray.direction.y < 0 != not_inside) {
		float t = (bounding_box_.min_.y - ray.origin.y) / ray.direction.y;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > bounding_box_.min_.z && temp.z < bounding_box_.max_.z) && (temp.x > bounding_box_.min_.x && temp.x < bounding_box_.max_.x) && t >= 0) {
			return  true;
		}
	}
	else if (ray.direction.y > 0 != not_inside) {
		float t = (bounding_box_.max_.y - ray.origin.y) / ray.direction.y;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.z > bounding_box_.min_.z && temp.z < bounding_box_.max_.z) && (temp.x > bounding_box_.min_.x && temp.x < bounding_box_.max_.x) && t >= 0) {
			return  true;
		}
	}

	if (ray.direction.z < 0 != not_inside) {
		float t = (bounding_box_.min_.z - ray.origin.z) / ray.direction.z;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.y > bounding_box_.min_.y && temp.y < bounding_box_.max_.y) && (temp.x > bounding_box_.min_.x && temp.x < bounding_box_.max_.x) && t >= 0) {
			return  true;
		}
	}
	else if (ray.direction.z > 0 != not_inside) {
		float t = (bounding_box_.max_.z - ray.origin.z) / ray.direction.z;
		glm::vec3 temp{ ray.origin + ray.direction * t };
		if ((temp.y > bounding_box_.min_.y && temp.y < bounding_box_.max_.y) && (temp.x > bounding_box_.min_.x && temp.x < bounding_box_.max_.x) && t >= 0) {
			return  true;
		}
	}

	return false;
}

std::ostream& Shape::print(std::ostream& os) const {
	return os << "Name: " << name_ << ", Material: " << mat_->name_ << "\n";
}
std::ostream& operator<<(std::ostream& os, Shape const& s) {
	return s.print(os);
}