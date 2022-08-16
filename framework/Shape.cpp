#include "Shape.hpp"
// Shape::Shape(std::string const& s, Color const& c): name_{ s }, color_{ c }{};

Shape::Shape(std::string const& s, std::shared_ptr<Material> const& m) : name_{ s }, mat_{ m } {};

Shape::Shape() {};

Shape::~Shape() {
	std::cout << "Destruktor Shape" << std::endl;
};

// Color Shape::get_color() const {
// 	return color_;
// }

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

void Shape::set_translation(Translation const& translation) {
	translation_ = translation;
	update_model_matrix();
}

void Shape::set_rotation(Rotation const& rot) {
	rotation_ = rot;
	update_model_matrix();
}

void Shape::set_scaling(Scaling const& scale) {
	scale_ = scale;
	update_model_matrix();
}

void Shape::update_model_matrix() {
	glm::mat4 translation_mat = glm::translate(glm::mat4(), translation_.translate);
	glm::mat4 rotation_mat = glm::rotate(rotation_.angle, rotation_.vector);
	// uniform scaling for now
	glm::mat4 scale_mat = glm::scale(glm::vec3(scale_.scale));

	model_ = translation_mat * rotation_mat * scale_mat;
}

glm::vec3 Shape::obj_to_world_position(glm::vec3 const& position) const{
	return glm::vec3(model_ * glm::vec4(position, 1.0f));
}

glm::vec3 Shape::obj_to_world_direction(glm::vec3 const& direction) const{
	return glm::vec3(model_ * glm::vec4(direction, 0.0f));
}

glm::vec3 Shape::world_to_obj_position(glm::vec3 const& position) const{
	return glm::vec3(glm::inverse(model_) * glm::vec4(position, 1.0f));
}

glm::vec3 Shape::world_to_obj_direction(glm::vec3 const& direction) const{
	return glm::vec3(glm::inverse(model_) * glm::vec4(direction, 0.0f));
}

std::ostream& Shape::print(std::ostream& os) const {
	return os << "Name: " << name_ << ", Material: " << mat_->name_ << "\n";
}
std::ostream& operator<<(std::ostream& os, Shape const& s) {
	return s.print(os);
}