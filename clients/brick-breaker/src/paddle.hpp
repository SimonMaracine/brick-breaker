#pragma once

#include <glm/glm.hpp>

struct Paddle {
    glm::vec3 get_dimensions() const;
    glm::vec3 get_position() const;
    constexpr glm::vec3 get_rotation() const { return glm::vec3(0.0f, glm::radians(90.0f), 0.0); }
    constexpr float get_scale() const { return 0.35f; }

    float position {0.0f};
    float velocity {0.0f};
};
