#include <engine/engine.hpp>

struct AboutScene : public bb::Scene {
    AboutScene()
        : bb::Scene("about") {}

    void on_enter() override;
    void on_exit() override;
    void on_update() override;

    void on_window_resized(const bb::WindowResizedEvent& event);
    void on_key_released(const bb::KeyReleasedEvent& event);

    bb::Camera2D cam_2d;
};
