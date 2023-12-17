#pragma once

#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>

#include "my_camera_controller.hpp"

struct LevelScene : public bb::Scene {
    LevelScene()
        : bb::Scene("level") {}

    virtual void on_enter() override;
    virtual void on_exit() override;
    virtual void on_update() override;

    void on_window_resized(bb::WindowResizedEvent& event);

    void load_platform();

    bb::Camera cam;
    MyCameraController cam_controller;

    bb::DirectionalLight directional_light;

    // Caches to easily store these resources
    resmanager::Cache<bb::VertexArray> cache_vertex_array;
    resmanager::Cache<bb::MaterialInstance> cache_material_instance;
    resmanager::Cache<bb::Texture> cache_texture;
};
