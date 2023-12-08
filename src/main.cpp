#include "engine/engine.hpp"

struct MainScene : public bb::Scene {
    MainScene()
        : bb::Scene("main") {}

    virtual void on_enter() override {
        connect_event<bb::KeyPressedEvent, &MainScene::on_key_pressed>(this);
        connect_event<bb::KeyReleasedEvent, &MainScene::on_key_released>(this);
        connect_event<bb::MouseMovedEvent, &MainScene::on_mouse_moved>(this);
        connect_event<bb::MouseButtonPressedEvent, &MainScene::on_mouse_button_pressed>(this);
        connect_event<bb::MouseButtonReleasedEvent, &MainScene::on_mouse_button_released>(this);
        connect_event<bb::MouseWheelScrolledEvent, &MainScene::on_mouse_wheel_scrolled>(this);

        bb::log_message("Entered main scene\n");
    }

    virtual void on_exit() override {
        bb::log_message("Exited main scene\n");
    }

    virtual void on_update() override {
        bb::log_message("%f - %f\n", get_delta(), get_fps());  // FIXME these don't seem accurate
    }

    void on_key_pressed(const bb::KeyPressedEvent& event) {
        bb::log_message("KeyPressed: %d\n", static_cast<int>(event.key));
    }

    void on_key_released(const bb::KeyReleasedEvent& event) {
        bb::log_message("KeyReleased: %d\n", static_cast<int>(event.key));
    }

    void on_mouse_moved(const bb::MouseMovedEvent& event) {
        bb::log_message("MouseMoved: %dx%d\n", event.x, event.y);
    }

    void on_mouse_button_pressed(const bb::MouseButtonPressedEvent& event) {
        bb::log_message("MouseButtonPressed: %d\n", static_cast<int>(event.button));
    }

    void on_mouse_button_released(const bb::MouseButtonReleasedEvent& event) {
        bb::log_message("MouseButtonReleased: %d\n", static_cast<int>(event.button));
    }

    void on_mouse_wheel_scrolled(const bb::MouseWheelScrolledEvent& event) {
        bb::log_message("MouseWheelScrolled: %d\n", event.scroll);
    }
};

int main() {
    bb::ApplicationProperties properties;

    try {
        bb::Application application {properties};
        application.add_scene<MainScene>();
        application.run("main");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
