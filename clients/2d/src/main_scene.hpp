#pragma once

#include <memory>
#include <cstdint>

#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>

using namespace resmanager::literals;

struct Main : public bb::Scene {
    Main()
        : bb::Scene("main") {}

    void on_enter() override;
    void on_exit() override;
    void on_update() override;

    void on_window_resized(const bb::WindowResizedEvent& event);

    void draw();

    std::uint32_t* pixels {nullptr};
    std::unique_ptr<bb::Texture> texture;
    std::unique_ptr<bb::VertexArray> vertex_array;
    std::unique_ptr<bb::Shader> shader;

    bb::TextureSpecification specification;
};
