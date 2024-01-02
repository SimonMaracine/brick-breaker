#pragma once

#include <glm/glm.hpp>

struct Sphere {
    glm::vec3 position {};
    float radius {};
};

struct Box {
    glm::vec3 position {};  // Center
    float width {};  // x
    float height {};  // y
    float depth {};  // z
};

enum class SphereBoxSide {
    FrontBack,
    LeftRight
};

bool collision_sphere_box(const Sphere& sphere, const Box& box);
SphereBoxSide sphere_box_side_2d(const Sphere& sphere, const Box& box);
