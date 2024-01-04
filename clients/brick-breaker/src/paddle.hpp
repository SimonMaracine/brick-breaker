#pragma once

#include <glm/glm.hpp>

class Paddle {
public:
    constexpr glm::vec3 get_dimensions() const { return glm::vec3(5.5f, 1.0f, 1.0f) * get_scale(); }
    constexpr glm::vec3 get_rotation() const { return glm::vec3(0.0f, glm::radians(90.0f), 0.0); }
    constexpr float get_scale() const { return 0.35f; }

    glm::vec3 get_position() const { return glm::vec3(position_x, 0.7f, 10.5f); }

    void set_position(float position) {
        position_x = position;
    }

    float velocity_x {0.0f};
private:
    float position_x {0.0f};

    // TODO variable width
};
