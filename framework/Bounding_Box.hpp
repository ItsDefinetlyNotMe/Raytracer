#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <glm/vec3.hpp>

struct Bounding_Box {
    glm::vec3 min_;
    glm::vec3 max_;
};

#endif // BOUNDING_BOX_HPP