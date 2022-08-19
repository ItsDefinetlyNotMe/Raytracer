#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <vector>
#include "Shape.hpp"

class Composite : public Shape {
private:
    std::vector<std::shared_ptr<Shape>> children_;
public:
	Composite(std::string const& n, std::vector<std::shared_ptr<Shape>> const& children);

	inline float area() const {return 0;};
	inline float volume() const {return 0;};
	// std::ostream& print(std::ostream& os) const override;
	Hitpoint intersect(Ray const& ray) const override;
	inline glm::vec3 normal(glm::vec3 const& point) const override {return glm::vec3(0);};
    Bounding_Box create_bounding_box() override;
};

#endif // COMPOSITE_HPP