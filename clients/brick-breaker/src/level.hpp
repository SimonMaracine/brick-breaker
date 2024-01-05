#pragma once

#include <unordered_map>
#include <string>
#include <optional>

#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>
#include <glm/glm.hpp>

#include "my_camera_controller.hpp"
#include "collision.hpp"
#include "paddle.hpp"
#include "ball.hpp"
#include "brick.hpp"
#include "orb.hpp"
#include "events.hpp"
#include "id_generator.hpp"

struct LevelScene : public bb::Scene {
    LevelScene()
        : bb::Scene("level") {}

    virtual void on_enter() override;
    virtual void on_exit() override;
    virtual void on_update() override;

    void on_window_resized(const bb::WindowResizedEvent& event);
    void on_key_pressed(const bb::KeyPressedEvent& event);
    void on_key_released(const bb::KeyReleasedEvent& event);

    void load_shaders();
    void load_platform();
    void load_ball();
    void load_paddle();
    void load_brick();
    void load_lamp();
    void load_orb();

    void update_collisions();
    void update_bricks();
    void update_paddle(Paddle& paddle);
    void update_orb(Orb& orb);
    void update_ball(Ball& ball);
    void shoot_balls();
    void create_ball();
    void spawn_orb(glm::vec3 position);
    void die();
    void win();
    void lose();
    static std::optional<std::unordered_map<unsigned int, Brick>> load_level(const std::string& file_path, IdGenerator& gen);
    glm::vec2 bounce_ball_off_paddle(const Ball& ball);
    float rotate_ball(const Ball& ball);
    void on_ball_paddle_collision(const BallPaddleCollisionEvent& event);
    void on_ball_miss(const BallMissEvent& event);
    void on_orb_miss(const OrbMissEvent& event);
    void on_orb_paddle_collision(const OrbPaddleCollisionEvent& event);
    void on_ball_brick_collision(const BallBrickCollisionEvent& event);

    void draw_bounding_box(const Box& box);
    void draw_fps();

    bb::Camera cam;
    MyCameraController cam_controller;
    bb::Camera2D cam_2d;

    bb::DirectionalLight directional_light;
    bb::PointLight lamp_left;
    bb::PointLight lamp_right;

    IdGenerator id_gen;

    Paddle paddle;
    std::unordered_map<unsigned int, Ball> balls;
    std::unordered_map<unsigned int, Brick> bricks;
    std::unordered_map<unsigned int, Orb> orbs;

    unsigned int lives {};
    int score {};

    enum class GameOver {
        None,
        Lost,
        Won
    } game_over {};

    // Caches to easily store these resources
    resmanager::Cache<bb::VertexArray> cache_vertex_array;
    resmanager::Cache<bb::MaterialInstance> cache_material_instance;
    resmanager::Cache<bb::Texture> cache_texture;
    resmanager::Cache<bb::Material> cache_material;
};
