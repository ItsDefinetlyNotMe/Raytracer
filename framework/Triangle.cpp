#include "Triangle.hpp"
#include "glm/gtx/intersect.hpp"
Triangle::Triangle(std::string const& n, std::shared_ptr<Material> const& mat, glm::vec3 lb, glm::vec3 rb, glm::vec3 t) : Shape(n, mat), left_bottom_{ lb }, right_bottom_{ rb }, top_{ t } {};

std::ostream& Triangle::print(std::ostream& os) const {
	return Shape::print(os) << "vertecie 0: (" << left_bottom_.x << "," << left_bottom_.y << "," << left_bottom_.z << ") " << "vertecie 1: (" << right_bottom_.x << "," << right_bottom_.y << "," << right_bottom_.z << ")" << "vertecie 2: (" << top_.x << "," << top_.y << "," << top_.z << ")";
}

Hitpoint Triangle::intersect(Ray const& r) const {
	glm::vec3 uvt; // wtf ??
	Ray obj_ray = { world_to_obj_position(r.origin),glm::normalize(world_to_obj_direction(r.direction)) };
	bool hit = glm::intersectRayTriangle(obj_ray.origin, obj_ray.direction, left_bottom_, right_bottom_, top_, uvt);
	float world_t = uvt.z * Shape::get_scale();
	glm::vec3 world_point_hit {r.origin + glm::normalize(r.direction) * world_t};
	glm::vec3 norm = normal(world_point_hit);
	return Hitpoint{hit,world_t,Shape::get_name(),Shape::get_material(), world_point_hit, r.direction, norm};
}

glm::vec3 Triangle::normal(glm::vec3 const& point) const {
	glm::vec3 a = obj_to_world_position(left_bottom_);
	glm::vec3 b = obj_to_world_position(right_bottom_);
	glm::vec3 c = obj_to_world_position(top_);
	return glm::normalize(glm::cross(b - a, c - a));
}

Bounding_Box Triangle::create_bounding_box() {
	// get all 8 transformed points and build bb from there

	glm::vec3 p1 = obj_to_world_position(left_bottom_);
	glm::vec3 p2 = obj_to_world_position(right_bottom_);
	glm::vec3 p3 = obj_to_world_position(top_);

	glm::vec3 min = p1;
	glm::vec3 max = p2;

	if (max.x < p1.x) max.x = p1.x;
	if (max.y < p1.y) max.y = p1.y;
	if (max.z < p1.z) max.z = p1.z;

	if (min.x > p2.x) min.x = p2.x;
	if (min.y > p2.y) min.y = p2.y;
	if (min.z > p2.z) min.z = p2.z;

	if (min.x > p3.x) min.x = p3.x;
	if (min.y > p3.y) min.y = p3.y;
	if (min.z > p3.z) min.z = p3.z;

	if (max.x < p3.x) max.x = p3.x;
	if (max.y < p3.y) max.y = p3.y;
	if (max.z < p3.z) max.z = p3.z;

	Bounding_Box bb {min, max};

	Shape::set_bounding_box(bb);
	return bb;
}