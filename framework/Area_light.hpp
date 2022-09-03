#ifndef AREA_LIGHT
#define AREA_LIGHT
#include <glm/vec3.hpp>
#include <vector>
#include <random>

#include "color.hpp"

struct Area_Light {
	std::string name;
	glm::vec3 pos_{};
	glm::vec3 u_vec_{};
	glm::vec3 v_vec_{};
	unsigned u_steps_ = 1;
	unsigned v_steps_ = 1;
	Color color_{ 1,1,1 };
	float brightness_ = 2;

	glm::vec3 sample(int x, int y) {
		glm::vec3 pos{ pos_ };
		pos += ((float)x * u_vec_ / (float)u_steps_) + ((std::rand()%11)/10.0f) * (u_vec_ / (float)u_steps_);
		pos += ((float)y * v_vec_ / (float)v_steps_) + ((std::rand()%11)/10.0f) * (v_vec_ / (float)v_steps_);
		return pos;
	};
};

#endif // AREA_LIGHT


