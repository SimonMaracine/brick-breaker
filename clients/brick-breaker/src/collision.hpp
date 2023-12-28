#pragma once

#include <glm/glm.hpp>

struct Sphere {
    glm::vec3 position {};
    float radius {};
};

struct Box {
    glm::vec3 position {};  // Lower-left-back
    float width {};  // x
    float height {};  // y
    float depth {};  // z
};

bool collision_sphere_box(const Sphere& sphere, const Box& box);
