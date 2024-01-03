#include "collision.hpp"

#include <cassert>

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection

enum Orientation {
    Collinear,
    Clockwise,
    CounterClockwise
};

static Orientation orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r) {
    const float result {(q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y)};

    if (result == 0.0f) {  // Never happens
        return Collinear;
    }

    return result > 0.0f ? Clockwise : CounterClockwise;
}

static bool intersection_segments(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2) {
    const Orientation o1 {orientation(p1, q1, p2)};
    const Orientation o2 {orientation(p1, q1, q2)};
    const Orientation o3 {orientation(p2, q2, p1)};
    const Orientation o4 {orientation(p2, q2, q1)};

    if (o1 != o2 && o3 != o4) {
        return true;
    }

    return false;
}

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

SphereBoxSide sphere_box_side_2d(const Sphere& sphere, const Box& box) {
    const glm::vec2 line1 {box.position.x, box.position.z};
    glm::vec2 line2 {sphere.position.x, sphere.position.z};
    line2 += (line2 - line1);  // Enlarge this segment a bit

    const glm::vec2 front1 {box.position.x - box.width, box.position.z + box.depth};
    const glm::vec2 front2 {box.position.x + box.width, box.position.z + box.depth};

    const glm::vec2 back1 {box.position.x - box.width, box.position.z - box.depth};
    const glm::vec2 back2 {box.position.x + box.width, box.position.z - box.depth};

    const glm::vec2 left1 {box.position.x - box.width, box.position.z - box.depth};
    const glm::vec2 left2 {box.position.x - box.width, box.position.z + box.depth};

    const glm::vec2 right1 {box.position.x + box.width, box.position.z - box.depth};
    const glm::vec2 right2 {box.position.x + box.width, box.position.z + box.depth};

    if (intersection_segments(line1, line2, front1, front2) || intersection_segments(line1, line2, back1, back2)) {
        return SphereBoxSide::FrontBack;
    }

    if (intersection_segments(line1, line2, left1, left2) || intersection_segments(line1, line2, right1, right2)) {
        return SphereBoxSide::LeftRight;
    }

    // FIXME do physics in a separate update function

    assert(false);
    return {};  // std::unreachable's only in c++23 :P
}
