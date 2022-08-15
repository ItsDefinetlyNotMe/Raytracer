#include "material_container.hpp"
#include "color.hpp"
/*
einem std::vector<std::shared_ptr<Material>>,
einem std::set<std::shared_ptr<Material>> und
einer std::map<std::string, std::shared_ptr<Material>>.
Implementieren Sie nun drei Suchfunktionen, jeweils den Namen eines Materials über -
gerben bekommen, und den std::shared_ptr<Material> des gesuchten Materials
zurückgibt, falls es existiert hat, ansonsten einen nullptr.
Achten Sie darauf, dass Sie für die effiziente Suche auf Ihrem std::set mittels
std::set::find(...) eine Vergleichsfunktion der Form */

std::shared_ptr<Material> find(std::map<std::string,std::shared_ptr<Material>> const& map, std::string const& mname) {
	auto i = map.find(mname);
	if (i == map.end())
		//return std::make_shared<Material>(nullptr);
		return nullptr;
	return i->second;
}
std::shared_ptr<Material> find(std::set<std::shared_ptr<Material>> const& set, std::string const& mname) {
	auto i = set.find(std::make_shared<Material>(Material{ mname,Color{},Color{},Color{},0 }));
	if (i == set.end())
		return nullptr;
	return *i;
}
bool operator<(std::shared_ptr<Material> const& lhs, std::shared_ptr<Material> const& rhs) {
	return lhs->name_ < rhs->name_;
};