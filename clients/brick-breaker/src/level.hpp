#pragma once

#include <unordered_map>

#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>

#include "my_camera_controller.hpp"
#include "collision.hpp"
#include "paddle.hpp"
#include "ball.hpp"
#include "events.hpp"

struct LevelScene : public bb::Scene {
    LevelScene()
        : bb::Scene("level") {}

    virtual void on_enter() override;
    virtual void on_exit() override;
    virtual void on_update() override;

    void on_window_resized(bb::WindowResizedEvent& event);
    void on_key_pressed(bb::KeyPressedEvent& event);
    void on_key_released(bb::KeyReleasedEvent& event);

    void load_shaders();
    void load_platform();
    void load_ball();
    void load_paddle();
    void load_brick();

    void update_collisions();
    void update_paddle(Paddle& paddle);
    void update_ball(Ball& ball);
    void on_ball_paddle_collision(BallPaddleCollisionEvent& event);

    void draw_bounding_box(const Box& box);

    bb::Camera cam;
    MyCameraController cam_controller;

    bb::DirectionalLight directional_light;

    Paddle paddle;
    std::unordered_map<unsigned int, Ball> balls;

    // Caches to easily store these resources
    resmanager::Cache<bb::VertexArray> cache_vertex_array;
    resmanager::Cache<bb::MaterialInstance> cache_material_instance;
    resmanager::Cache<bb::Texture> cache_texture;
    resmanager::Cache<bb::Material> cache_material;
};
