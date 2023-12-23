#include <glm/glm.hpp>

#include "collision.hpp"

bool collision_sphere_cube(const Sphere& sphere, const Cube& cube) {
    const float cube_min_x {cube.position.x};
    const float cube_max_x {cube.position.x + cube.width};
    const float cube_min_y {cube.position.y};
    const float cube_max_y {cube.position.x + cube.height};
    const float cube_min_z {cube.position.z};
    const float cube_max_z {cube.position.x + cube.depth};

    const float x {glm::max(cube_min_x, glm::min(sphere.position.x, cube_max_x))};
    const float y {glm::max(cube_min_y, glm::min(sphere.position.y, cube_max_y))};
    const float z {glm::max(cube_min_z, glm::min(sphere.position.z, cube_max_z))};

    const float distance {glm::sqrt(
        (x - sphere.position.x) * (x - sphere.position.x) +
        (y - sphere.position.y) * (y - sphere.position.y) +
        (z - sphere.position.z) * (z - sphere.position.z)
    )};

    return distance < sphere.radius;
}
