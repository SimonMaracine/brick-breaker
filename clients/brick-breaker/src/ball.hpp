#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "constants.hpp"

class Ball {
public:
    Ball() = default;
    Ball(unsigned int index)
        : index(index) {}

    glm::vec3 get_position() const { return glm::vec3(position.x, GROUND_LEVEL + radius, position.z); }
    unsigned int get_index() const { return index; }

    void set_position_x(float position) {
        this->position.x = position;
    }

    void set_position_z(float position) {
        this->position.z = position;
    }

    glm::vec3 velocity {0.0f, 0.0f, 0.01f};  // Velocity must never be zero or else it completely messes up the transformation
    glm::quat rotation {1.0f, 0.0f, 0.0f, 0.0f};

    float radius {BALL_RADIUS_NORMAL};
    bool attached_to_paddle {true};
    bool fire {false};

    glm::mat4 transformation {1.0f};
private:
    unsigned int index {};
    glm::vec3 position {};
};
