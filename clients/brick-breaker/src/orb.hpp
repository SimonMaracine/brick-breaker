#pragma once

#include <glm/glm.hpp>

enum class OrbType {
    SpeedUp,     // green
    SpeedDown,   // blue
    ExtraLife,   // yellow
    Die,         // black
    FireBall,    // red
    NormalBall,  // grayish
    ExtraBall,   // pink

    FIRST = SpeedUp,
    LAST = ExtraBall
};

inline const glm::vec3 ORB_COLORS[static_cast<int>(OrbType::LAST) + 1] {
    glm::vec3(0.1f, 0.9f, 0.1f),
    glm::vec3(0.1f, 0.1f, 0.9f),
    glm::vec3(0.9f, 0.9f, 0.1f),
    glm::vec3(0.09f, 0.09f, 0.09f),
    glm::vec3(1.0f, 0.1f, 0.1f),
    glm::vec3(0.8f, 0.75f, 0.75f),
    glm::vec3(1.0f, 0.0f, 1.0f)
};

inline const int ORB_POINTS[static_cast<int>(OrbType::LAST) + 1] {
    75,
    55,
    25,
    350,
    20,
    80,
    15
};

class Orb {
public:
    Orb() = default;
    Orb(unsigned int index, float x, float z, glm::vec3 velocity, OrbType type)
        : position(glm::vec3(x, GROUND_LEVEL + radius + 0.2f, z)), velocity(velocity), index(index), type(type) {}

    unsigned int get_index() const { return index; }
    OrbType get_type() const { return type; }
    int get_points() const { return ORB_POINTS[static_cast<int>(type)]; }

    float radius {0.21f};

    glm::vec3 position {};
    glm::vec3 velocity {};
private:
    unsigned int index {};

    OrbType type {};
};
