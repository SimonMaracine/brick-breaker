#pragma once

#include <cstddef>

struct BallPaddleCollisionEvent {
    unsigned int ball_index {};
};

struct BallMissEvent {
    unsigned int ball_index {};
};

struct BallBrickCollisionEvent {
    unsigned int ball_index {};
    std::size_t brick_index {};
};
