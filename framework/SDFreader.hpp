#ifndef SDF_READER
#define SDF_READER

#include <vector>
#include "Material.hpp"
std::vector<std::shared_ptr<Material>> sdf_reader(std::string path);
#endif // !SDF_READER
