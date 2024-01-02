#include <cassert>

#include <glm/glm.hpp>

#include "collision.hpp"

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection

#define Point glm::vec2

/*
    #begin ChatGPT stuff (modified slightly)
*/
static bool onSegment(Point p, const Point& q, const Point& r) {
    return (q.x <= glm::max(p.x, r.x) && q.x >= glm::min(p.x, r.x) &&
            q.y <= glm::max(p.y, r.y) && q.y >= glm::min(p.y, r.y));
}

static int orientation(Point p, const Point& q, const Point& r) {
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0; // Collinear
    return (val > 0) ? 1 : 2; // Clockwise or counterclockwise
}

static bool doIntersect(Point p1, const Point& q1, Point p2, const Point& q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special cases

    // p1, q1, and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1, and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2, and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2, and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}
/*
    #end ChatGPT stuff
*/

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
    const glm::vec2 point1 {box.position};
    const glm::vec2 point2 {sphere.position};

    const glm::vec2 front1 {box.position.x - box.width, box.position.z + box.depth};
    const glm::vec2 front2 {box.position.x + box.width, box.position.z + box.depth};

    const glm::vec2 back1 {box.position.x - box.width, box.position.z - box.depth};
    const glm::vec2 back2 {box.position.x + box.width, box.position.z - box.depth};

    const glm::vec2 left1 {box.position.x - box.width, box.position.z - box.depth};
    const glm::vec2 left2 {box.position.x - box.width, box.position.z + box.depth};

    const glm::vec2 right1 {box.position.x + box.width, box.position.z - box.depth};
    const glm::vec2 right2 {box.position.x + box.width, box.position.z + box.depth};

    if (doIntersect(point1, point2, front1, front2) || doIntersect(point1, point2, back1, back2)) {
        return SphereBoxSide::FrontBack;
    }

    if (doIntersect(point1, point2, left1, left2) || doIntersect(point1, point2, right1, right2)) {
        return SphereBoxSide::LeftRight;
    }

    assert(false);
    return {};  // std::unreachable's only in c++23 :P
}
