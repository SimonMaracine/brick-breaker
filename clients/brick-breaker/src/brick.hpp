#pragma once

#include <glm/glm.hpp>

#include "constants.hpp"

struct Brick {
    Brick() = default;
    Brick(glm::vec3 position)
        : position(position) {}

    constexpr glm::vec3 get_dimensions() const { return glm::vec3(2.0f, 1.0f, 1.0f) * get_scale(); }
    constexpr float get_scale() const { return 0.35f; }

    glm::vec3 position {0.0f, GROUND_LEVEL, 0.0f};
};
