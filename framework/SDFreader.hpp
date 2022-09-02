#ifndef SDF_READER
#define SDF_READER

#include <vector>

#include "renderer.hpp"
#include "material_container.hpp"

#include "Composite.hpp"
#include "Box.hpp"
#include "Sphere.hpp"
#include "Cone.hpp"
#include "Cylinder.hpp"
#include "Triangle.hpp"

//std::vector<std::shared_ptr<Material>> sdf_reader(std::string path);
std::shared_ptr<Renderer> sdf_reader(std::string const& path);

#endif // !SDF_READER
