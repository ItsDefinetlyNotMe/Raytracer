#include "Composite.hpp"

Composite::Composite(std::string const& n, std::vector<std::shared_ptr<Shape>> const& children) : Shape{ n }, children_(children) {}

Hitpoint Composite::intersect(Ray const& ray) const {
    Hitpoint hitpoint;

    if (!Shape::intersect_bounding_box(ray)) return hitpoint;

    for (auto const& child : children_) {
        Hitpoint temp = child->intersect(ray);
        if (temp.hit && temp.t < hitpoint.t) hitpoint = temp;
    }

    return hitpoint;
}

void Composite::create_bounding_box() {
    bool first = true;
    Bounding_Box bb;

    for (auto const& child : children_) {
        Bounding_Box c_bb = child->get_bounding_box();
        if (first) {
            bb = Bounding_Box{c_bb.min_, c_bb.max_};
            first = false;
        } else {
            if (bb.min_.x > c_bb.min_.x) bb.min_.x = c_bb.min_.x;
            if (bb.min_.y > c_bb.min_.y) bb.min_.y = c_bb.min_.y;
            if (bb.min_.z > c_bb.min_.z) bb.min_.z = c_bb.min_.z;
            if (bb.max_.x < c_bb.max_.x) bb.max_.x = c_bb.max_.x;
            if (bb.max_.y < c_bb.max_.y) bb.max_.y = c_bb.max_.y;
            if (bb.max_.z < c_bb.max_.z) bb.max_.z = c_bb.max_.z;
        }
    }

    Shape::set_bounding_box(bb);
}

void Composite::prepare_for_rendering(glm::mat4 const& parent_world_mat, float parent_scale) {
	// turn local model matrix into global model matrix
	Shape::update_model_matrix(parent_world_mat, parent_scale);

    glm::mat4 model_mat = Shape::get_model_matrix();

    for (auto const& child : children_) {
        child->prepare_for_rendering(model_mat, parent_scale * get_scale());
    }

	// create bounding boxes in global world;
	create_bounding_box();
}