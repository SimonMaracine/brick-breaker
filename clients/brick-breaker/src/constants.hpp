#pragma once

#include <glm/glm.hpp>

inline constexpr float GROUND_LEVEL {0.307f};
inline constexpr float DEADLINE_Z {10.55f};
inline constexpr float PLATFORM_EDGE_MIN_X {-9.8f};
inline constexpr float PLATFORM_EDGE_MAX_X {9.8f};
inline constexpr float PLATFORM_EDGE_MIN_Z {-9.8f};
inline constexpr glm::vec3 GREEN {0.0f, 1.0f, 0.0f};
inline constexpr glm::vec3 LAMP_LEFT_POSITION {PLATFORM_EDGE_MIN_X, 2.5f, PLATFORM_EDGE_MIN_Z};
inline constexpr glm::vec3 LAMP_RIGHT_POSITION {PLATFORM_EDGE_MAX_X, 2.5f, PLATFORM_EDGE_MIN_Z};
inline constexpr float SHOOT_VELOCITY_Z {6.0f};
inline constexpr int BRICKS_GRID_MIN_X {-4};
inline constexpr int BRICKS_GRID_MAX_X {4};
inline constexpr int BRICKS_GRID_MIN_Z {-8};
inline constexpr int BRICKS_GRID_MAX_Z {7};
inline constexpr int BRICKS_GRID_MIN_Y {0};
inline constexpr int BRICKS_GRID_MAX_Y {4};
inline constexpr float PADDLE_VELOCITY {12.0f};
