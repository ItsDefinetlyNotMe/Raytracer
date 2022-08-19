#include "Composite.hpp"

Composite::Composite(std::string const& n, std::vector<std::shared_ptr<Shape>> children) : Shape{ n }, children_(children) {}

Hitpoint Composite::intersect(Ray const& ray) const {
    Hitpoint hitpoint;
    if (!Shape::intersect_bounding_box(ray)) return hitpoint;

    Ray comp_ray = Ray{world_to_obj_position(ray.origin), glm::normalize(world_to_obj_direction(ray.direction))};
	
    for (auto const& child : children_) {
        Hitpoint temp = child->intersect(comp_ray);
        if (temp.hit && temp.t < hitpoint.t) hitpoint = temp;
    }

    return hitpoint;
}

Bounding_Box Composite::create_bounding_box() {
    bool first = true;
    Bounding_Box bb;
    for (auto const& child : children_) {
        Bounding_Box c_bb = child->create_bounding_box();
        if (first) bb = c_bb;
        else {
            if (bb.min_.x > c_bb.min_.x) bb.min_.x = c_bb.min_.x;
            if (bb.min_.y > c_bb.min_.y) bb.min_.x = c_bb.min_.y;
            if (bb.min_.z > c_bb.min_.z) bb.min_.x = c_bb.min_.z;
            if (bb.max_.x < c_bb.max_.x) bb.max_.x = c_bb.max_.x;
            if (bb.max_.y < c_bb.max_.y) bb.max_.x = c_bb.max_.y;
            if (bb.max_.z < c_bb.max_.z) bb.max_.x = c_bb.max_.z;
        }
    }
    Shape::set_bounding_box(bb);
    return bb;
}