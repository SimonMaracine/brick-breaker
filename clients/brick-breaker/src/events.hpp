#pragma once

struct BallPaddleCollisionEvent {
    unsigned int ball_index {};
    SphereBoxSide side {};
};

struct BallMissEvent {
    unsigned int ball_index {};
};

struct BallBrickCollisionEvent {
    unsigned int ball_index {};
    unsigned int brick_index {};
    SphereBoxSide side {};
};
