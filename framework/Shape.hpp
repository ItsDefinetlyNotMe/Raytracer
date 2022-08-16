//composit patern
#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "Hitpoint.hpp"
#include "Ray.hpp"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>
#include <string>
#include <memory>
#include "Material.hpp"
#include "Transforms.hpp"
class Shape {
private:
	std::string name_{ "" };
	std::shared_ptr<Material> mat_;
	Translation translation_;
	Rotation rotation_;
	Scaling scale_;
	glm::mat4 model_;
public:
	// Shape(std::string const& s, Color const& c);
	Shape(std::string const& s, std::shared_ptr<Material> const& m);
	Shape();
	virtual ~Shape();


	virtual float area() const = 0;
	virtual float volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
	// Color get_color() const;

	std::shared_ptr<Material> get_material() const;
	std::string get_name() const;
	glm::mat4 get_model_matrix() const;
	float get_scale() const; // helpful for t

	void set_translation(Translation const& translation);
	void set_rotation(Rotation const& rot);
	void set_scaling(Scaling const& scale);

	void update_model_matrix();

	glm::vec3 obj_to_world_position(glm::vec3 const& position) const;
	glm::vec3 obj_to_world_direction(glm::vec3 const& direction) const;

	glm::vec3 world_to_obj_position(glm::vec3 const& position) const;
	glm::vec3 world_to_obj_direction(glm::vec3 const& direction) const;

	virtual Hitpoint intersect(Ray const& ray) const = 0;
	virtual glm::vec3 normal(glm::vec3 const& point) const = 0;
};
std::ostream& operator<<(std::ostream& os, Shape const& s);
#endif // ! SHAPE_HPP
