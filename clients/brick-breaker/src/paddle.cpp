#include <glm/glm.hpp>

#include "paddle.hpp"

glm::vec3 Paddle::get_dimensions() const {
    return glm::vec3(2.0f, 12.0f, 1.0f) * get_scale();
}

glm::vec3 Paddle::get_position() const {
    return glm::vec3(position, 0.7f, 10.5f);
}
