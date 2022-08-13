#include "color.hpp"
bool operator==(Color const& c1, Color const& c2){
return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

Color operator*(Color const& a, glm::vec3 const& b) {
	return Color{ a.r * b.x,a.g * b.y, a.b * b.z };
}

Color operator*(glm::vec3 const& a, Color const& b) {
	return b*a;
}

Color operator*(Color const& a, float b) {
	return Color{ a.r * b, a.g * b,a.b * b };

}

Color operator*(float a, Color const& b) {
	return b * a;
}

Color operator*(Color const& a, Color const& b) {
	return { a.r * b.r,a.g * b.g,a.b * b.b };
}
