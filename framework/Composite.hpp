#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <vector>

#include "Shape.hpp"

class Composite : public Shape {
private:
    std::vector<std::shared_ptr<Shape>> children_;
public:
	Composite(std::string const& n, std::vector<std::shared_ptr<Shape>> const& children);

	// std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& ray) const override;
	inline glm::vec3 normal(glm::vec3 const& point) const override {return glm::vec3(0);};
    void create_bounding_box() override;
	void prepare_for_rendering(glm::mat4 const& parent_world_mat, float parent_float) override;
};

#endif // COMPOSITE_HPP