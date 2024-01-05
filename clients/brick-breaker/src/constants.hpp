#pragma once

#include <glm/glm.hpp>

inline constexpr float GROUND_LEVEL {0.305f};
inline constexpr float DEADLINE_Z {10.55f};
inline constexpr float PLATFORM_EDGE_MIN_X {-9.8f};
inline constexpr float PLATFORM_EDGE_MAX_X {9.8f};
inline constexpr float PLATFORM_EDGE_MIN_Z {-9.8f};
inline const glm::vec3 GREEN {0.0f, 1.0f, 0.0f};
inline const glm::vec3 LAMP_LIGHT_LEFT_POSITION {PLATFORM_EDGE_MIN_X, 2.5f, PLATFORM_EDGE_MIN_Z};
inline const glm::vec3 LAMP_LIGHT_RIGHT_POSITION {PLATFORM_EDGE_MAX_X, 2.5f, PLATFORM_EDGE_MIN_Z};
inline const glm::vec3 LAMP_LEFT_POSITION {LAMP_LIGHT_LEFT_POSITION - glm::vec3(0.0f, 1.15f, 0.0f)};
inline const glm::vec3 LAMP_RIGHT_POSITION {LAMP_LIGHT_RIGHT_POSITION - glm::vec3(0.0f, 1.15f, 0.0f)};
inline constexpr float SHOOT_VELOCITY {7.3f};
inline constexpr int BRICKS_GRID_MIN_X {-4};
inline constexpr int BRICKS_GRID_MAX_X {4};
inline constexpr int BRICKS_GRID_MIN_Z {-8};
inline constexpr int BRICKS_GRID_MAX_Z {7};
inline constexpr int BRICKS_GRID_MIN_Y {0};
inline constexpr int BRICKS_GRID_MAX_Y {4};
inline constexpr float PADDLE_VELOCITY {12.0f};
inline constexpr float BALL_RADIUS_NORMAL {0.3f};
inline constexpr float BALL_RADIUS_FIRE {0.4f};
inline constexpr float ORB_RATE {0.82f};
