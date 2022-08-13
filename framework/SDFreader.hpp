#ifndef SDF_READER
#define SDF_READER

#include <vector>
#include "Material.hpp"

#include "renderer.hpp"
#include "renderer.hpp"
#include "Point_light.hpp"
#include "material_container.hpp"
#include "Shape.hpp"
#include "Scene.hpp"

//std::vector<std::shared_ptr<Material>> sdf_reader(std::string path);
std::shared_ptr<Renderer> sdf_reader(std::string const& path);

#endif // !SDF_READER
