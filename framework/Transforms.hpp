#ifndef TRANSFORMS
#define TRANSFORMS

#include <glm/vec3.hpp>

struct Translation {
    glm::vec3 translate {0.0f, 0.0f, 0.0f};
};

struct Rotation {
    float angle = 0.0f;
    glm::vec3 vector {0.0f, 0.0f, 0.0f};
};

struct Scaling {
    float scale = 1.0f;
};

#endif // TRANSFORMS