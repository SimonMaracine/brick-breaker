#pragma once

#include <glm/glm.hpp>

class Paddle {
public:
    static glm::vec3 get_dimensions() { return glm::vec3(5.5f, 1.0f, 1.0f) * get_scale(); }
    static glm::vec3 get_rotation() { return glm::vec3(0.0f, glm::radians(90.0f), 0.0); }
    static float get_scale() { return 0.35f; }

    glm::vec3 get_position() const { return glm::vec3(position_x, 0.7f, 10.5f); }

    void set_position(float position) {
        position_x = position;
    }

    float velocity_x {0.0f};
private:
    float position_x {0.0f};

    // TODO variable width
};
