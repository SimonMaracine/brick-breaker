#include <glm/glm.hpp>

#include "collision.hpp"

bool collision_sphere_box(const Sphere& sphere, const Box& box) {
    const float box_min_x {box.position.x};
    const float box_max_x {box.position.x + box.width};
    const float box_min_y {box.position.y};
    const float box_max_y {box.position.x + box.height};
    const float box_min_z {box.position.z};
    const float box_max_z {box.position.x + box.depth};

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
