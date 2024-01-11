#pragma once

#include <engine/engine.hpp>

#include "menu_selection.hpp"

struct MenuScene : public bb::Scene {
    MenuScene()
        : bb::Scene("menu") {}

    virtual void on_enter() override;
    virtual void on_exit() override;
    virtual void on_update() override;

    void on_window_resized(const bb::WindowResizedEvent& event);
    void on_key_released(const bb::KeyReleasedEvent& event);

    void load_font();
    void load_sounds();

    void execute_selection(int selection);
    void draw_version();

    bb::Camera2D cam_2d;

    enum {
        AdventureMode,
        CustomLevels,
        About,
        Quit,
        COUNT
    };

    MenuSelection<int> menu_selection {COUNT};

    static constexpr unsigned int VER_MAJOR {0};
    static constexpr unsigned int VER_MINOR {1};
    static constexpr unsigned int VER_PATCH {0};
};
