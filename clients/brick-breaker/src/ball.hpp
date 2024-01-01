#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "constants.hpp"

struct Ball {
    Ball() = default;
    Ball(unsigned int index)
        : index(index) {}

    unsigned int index {};

    glm::vec3 position {0.0f, GROUND_LEVEL, 0.0f};
    glm::vec3 velocity {2.0f, 0.0f, 3.0f};
    glm::quat rotation {1.0f, 0.0f, 0.0f, 0.0f};

    float radius {0.25f};

    glm::mat4 transformation {1.0f};
};
