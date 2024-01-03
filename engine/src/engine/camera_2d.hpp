#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace bb {
    struct Camera2D {
        void set_projection_matrix(float left, float right, float bottom, float top) {
            projection_matrix = glm::ortho(left, right, bottom, top);
        }

        glm::mat4 projection_matrix {glm::mat4(1.0f)};
    };
}
