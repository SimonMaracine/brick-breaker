#pragma once

#include <glm/glm.hpp>

struct Sphere {
    glm::vec3 position {};
    float radius {};
};

struct Cube {
    glm::vec3 position {};  // Lower-left-back
    float width {};  // x
    float height {};  // y
    float depth {};  // z
};

bool collision_sphere_cube(const Sphere& sphere, const Cube& cube);
