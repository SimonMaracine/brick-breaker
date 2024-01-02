#pragma once

#include <glm/glm.hpp>

inline constexpr float GROUND_LEVEL {0.65f};
inline constexpr float DEADLINE_Z {10.55f};
inline constexpr float PLATFORM_EDGE_MIN_X {-9.8f};
inline constexpr float PLATFORM_EDGE_MAX_X {9.8f};
inline constexpr float PLATFORM_EDGE_MIN_Z {-9.8f};
inline constexpr glm::vec3 GREEN {0.0f, 1.0f, 0.0f};
inline constexpr float SHOOT_VELOCITY_Z {6.0f};
inline constexpr int BRICKS_GRID_MIN_X {-4};
inline constexpr int BRICKS_GRID_MAX_X {4};
inline constexpr int BRICKS_GRID_MIN_Z {-8};
inline constexpr int BRICKS_GRID_MAX_Z {8};
