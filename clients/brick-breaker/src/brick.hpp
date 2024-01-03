#pragma once

#include <glm/glm.hpp>

#include "constants.hpp"

enum class BrickType {
    Pink,
    Blue,
    Yellow,
    FIRST = Pink,
    LAST = Yellow
};

struct Brick {
    Brick() = default;
    Brick(unsigned int index, glm::ivec3 grid, BrickType type)
        : index(index), grid(grid), position(grid_to_position(grid)), type(type) {}

    constexpr glm::vec3 get_dimensions() const { return glm::vec3(2.0f, 1.0f, 1.0f) * get_scale(); }
    constexpr float get_scale() const { return 0.5f; }

    glm::vec3 grid_to_position(glm::ivec3 grid) const {
        return glm::vec3(
            static_cast<float>(grid.x) * get_dimensions().x * 2.0f,
            static_cast<float>(grid.y) * get_dimensions().y * 2.0f + GROUND_LEVEL,
            static_cast<float>(grid.z) * get_dimensions().z * 2.0f
        );
    }

    unsigned int index {};

    glm::ivec3 grid {};
    glm::vec3 position {0.0f, GROUND_LEVEL, 0.0f};

    BrickType type {};
};
