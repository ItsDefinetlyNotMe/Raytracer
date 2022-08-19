#include "Composite.hpp"

Composite::Composite(std::string const& n, std::vector<std::shared_ptr<Shape>> const& children) : Shape{ n }, children_(children) {}

Hitpoint Composite::intersect(Ray const& ray) const {
    Hitpoint hitpoint;

    Ray comp_ray = Ray{world_to_obj_position(ray.origin), glm::normalize(world_to_obj_direction(ray.direction))};

    if (!Shape::intersect_bounding_box(ray)) return hitpoint;

    for (auto const& child : children_) {
        Hitpoint temp = child->intersect(comp_ray);
        if (temp.hit && temp.t < hitpoint.t) hitpoint = temp;
    }

    // we want the information in the correct space
    hitpoint.point3d = obj_to_world_position(hitpoint.point3d);
    hitpoint.direction = obj_to_world_direction(hitpoint.direction);
    hitpoint.normal = obj_to_world_direction(hitpoint.normal);
    return hitpoint;
}

Bounding_Box Composite::create_bounding_box() {
    bool first = true;
    Bounding_Box bb;
    for (auto const& child : children_) {
        Bounding_Box c_bb = child->create_bounding_box();
        c_bb.min_ = obj_to_world_position(c_bb.min_);
        c_bb.max_ = obj_to_world_position(c_bb.max_);

        glm::vec3 points[8]; // c-style array should be fine
        points[0] = obj_to_world_position(c_bb.min_);
        points[1] = obj_to_world_position(glm::vec3(c_bb.min_.x, c_bb.min_.y, c_bb.max_.z));
        points[2] = obj_to_world_position(glm::vec3(c_bb.min_.x, c_bb.max_.y, c_bb.min_.z));
        points[3] = obj_to_world_position(glm::vec3(c_bb.min_.x, c_bb.max_.y, c_bb.max_.z));
        points[4] = obj_to_world_position(glm::vec3(c_bb.max_.x, c_bb.min_.y, c_bb.min_.z));
        points[5] = obj_to_world_position(glm::vec3(c_bb.max_.x, c_bb.min_.y, c_bb.max_.z));
        points[6] = obj_to_world_position(glm::vec3(c_bb.max_.x, c_bb.max_.y, c_bb.min_.z));
        points[7] = obj_to_world_position(c_bb.max_);

        for (int i = 0; i < 8; ++i) {
            if (c_bb.min_.x > points[i].x) c_bb.min_.x = points[i].x;
            if (c_bb.min_.y > points[i].y) c_bb.min_.y = points[i].y;
            if (c_bb.min_.z > points[i].z) c_bb.min_.z = points[i].z;
            if (c_bb.max_.x < points[i].x) c_bb.max_.x = points[i].x;
            if (c_bb.max_.y < points[i].y) c_bb.max_.y = points[i].y;
            if (c_bb.max_.z < points[i].z) c_bb.max_.z = points[i].z;
        }

        if (first) {
            bb = c_bb;
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
    return bb;
}