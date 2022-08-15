#ifndef MAT_CONTAINER
#define MAT_CONTAINER

#include "Material.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>
//std::shared_ptr<Material> find(std::vector<std::shared_ptr<Material>> const& vec, std::string const& mname);
template <typename T>
std::shared_ptr<T> find(std::vector<std::shared_ptr<T>> const& vec, std::string const& mname) {
	for (auto i = vec.begin(); i != vec.end(); ++i) {
		if ((*i)->name_ == mname) {
			return (*i);
		}
	}
	return nullptr;
};

std::shared_ptr<Material> find(std::map<std::string, std::shared_ptr<Material>> const& vec, std::string const& mname);
std::shared_ptr<Material> find(std::set<std::shared_ptr<Material>> const& vec, std::string const& mname);
bool operator<(std::shared_ptr<Material> const& lhs, std::shared_ptr<Material> const& rhs);
#endif // !MAT_CONTAINER

