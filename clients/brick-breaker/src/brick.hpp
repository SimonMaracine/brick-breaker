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

class Brick {
public:
    Brick() = default;
    Brick(unsigned int index, glm::ivec3 grid, BrickType type)
        : index(index), grid(grid), type(type) {
        set_position(grid);
    }

    static glm::vec3 get_dimensions() { return glm::vec3(2.0f, 1.0f, 1.0f) * get_scale(); }
    static float get_scale() { return 0.5f; }

    glm::vec3 get_position() const { return position; }
    glm::ivec3 get_grid() const { return grid; }
    BrickType get_type() const { return type; }

    int get_points() const {
        static constexpr int values[] { 10, 15, 5 };
        return values[static_cast<int>(type)];
    }

    void set_position(glm::ivec3 grid) {
        position = glm::vec3(
            static_cast<float>(grid.x) * get_dimensions().x * 2.0f,
            static_cast<float>(grid.y) * get_dimensions().y * 2.0f + GROUND_LEVEL + get_dimensions().y,
            static_cast<float>(grid.z) * get_dimensions().z * 2.0f
        );
    }

    void lower_grid() {
        grid.y--;
    }
private:
    unsigned int index {};

    glm::ivec3 grid {};
    glm::vec3 position {};

    BrickType type {};
};
