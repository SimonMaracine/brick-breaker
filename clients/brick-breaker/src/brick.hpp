#pragma once

#include <glm/glm.hpp>

#include "constants.hpp"

enum class BrickType {
    Pink,
    Blue,
    Yellow
};

struct Brick {
    Brick() = default;
    Brick(int x, int z, BrickType type)
        : position(glm::vec3(x * get_dimensions().x * 2.0f, GROUND_LEVEL, z * get_dimensions().z * 2.0f)), type(type) {}

    constexpr glm::vec3 get_dimensions() const { return glm::vec3(2.0f, 1.0f, 1.0f) * get_scale(); }
    constexpr float get_scale() const { return 0.5f; }

    glm::vec3 position {0.0f, GROUND_LEVEL, 0.0f};

    BrickType type {};
};
