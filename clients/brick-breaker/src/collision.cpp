#include <glm/glm.hpp>

#include "collision.hpp"

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection

bool collision_sphere_box(const Sphere& sphere, const Box& box) {
    const float box_min_x {box.position.x - box.width};
    const float box_max_x {box.position.x + box.width};
    const float box_min_y {box.position.y - box.height};
    const float box_max_y {box.position.y + box.height};
    const float box_min_z {box.position.z - box.depth};
    const float box_max_z {box.position.z + box.depth};

    const float x {glm::max(box_min_x, glm::min(sphere.position.x, box_max_x))};
    const float y {glm::max(box_min_y, glm::min(sphere.position.y, box_max_y))};
    const float z {glm::max(box_min_z, glm::min(sphere.position.z, box_max_z))};

    const float distance {glm::sqrt(
        (x - sphere.position.x) * (x - sphere.position.x) +
        (y - sphere.position.y) * (y - sphere.position.y) +
        (z - sphere.position.z) * (z - sphere.position.z)
    )};

    return distance < sphere.radius;
}
