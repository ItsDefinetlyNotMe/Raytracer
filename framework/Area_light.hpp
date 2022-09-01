#ifndef AREA_LIGHT
#define AREA_LIGHT
#include <glm/vec3.hpp>
#include "color.hpp"
#include <vector>
#include <random>
struct Area_Light {
	glm::vec3 pos_{};
	glm::vec3 v_vec_{};
	glm::vec3 u_vec_{};
	unsigned v_steps_ = 0;
	unsigned u_steps_ = 0;
	//muss noch in die sdf
	Color color_{ 1,1,1 };
	float brightness_ = 4;

	glm::vec3 sample(int x, int y) {
		glm::vec3 pos{ pos_ };
		pos += ((float)x * v_vec_ / (float)v_steps_) + ((std::rand()%11)/10.0f) * (v_vec_ / (float)v_steps_);
		pos += ((float)y * u_vec_ / (float)u_steps_) + ((std::rand() % 11) / 10.0f) * (u_vec_ / (float)u_steps_);
		return pos;
	};
};

#endif // !AREA_LIGHT


