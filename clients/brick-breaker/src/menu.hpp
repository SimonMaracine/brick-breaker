#pragma once

#include <vector>
#include <string>
#include <utility>
#include <optional>
#include <cstddef>

#include <engine/engine.hpp>

#include "data.hpp"

struct Menu : public bb::Scene {
    Menu()
        : bb::Scene("menu") {}

    virtual void on_enter() override;
    virtual void on_exit() override;
    virtual void on_update() override;

    void on_window_resized(const bb::WindowResizedEvent& event);
    void on_key_released(const bb::KeyReleasedEvent& event);

    void load_font();
    void reload_levels();
    void load_sounds();

    std::optional<std::string> get_level_name(const std::string& file_path);

    void draw_version();

    bb::Camera2D cam_2d;

    std::vector<std::pair<std::string, std::string>> level_paths;
    int level_index {0};  // Must be unsigned

    static constexpr int MAX_LEVELS {7};

    static constexpr unsigned int VER_MAJOR {0};
    static constexpr unsigned int VER_MINOR {1};
    static constexpr unsigned int VER_PATCH {0};
};
