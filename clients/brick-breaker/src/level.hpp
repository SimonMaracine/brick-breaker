#pragma once

#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "my_camera_controller.hpp"

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

    bb::Camera cam;
    MyCameraController cam_controller;

    bb::DirectionalLight directional_light;

    glm::vec3 ball_position {};
    glm::vec3 ball_velocity {};
    glm::quat ball_rotation {};
    float paddle_position {};
    float paddle_velocity {};

    // Caches to easily store these resources
    resmanager::Cache<bb::VertexArray> cache_vertex_array;
    resmanager::Cache<bb::MaterialInstance> cache_material_instance;
    resmanager::Cache<bb::Texture> cache_texture;
    resmanager::Cache<bb::Material> cache_material;
};
