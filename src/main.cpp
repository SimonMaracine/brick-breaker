#include <iostream>

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

    }

    void on_key_pressed(const bb::KeyPressedEvent& event) {
        std::cout << "KeyPressed: " << static_cast<int>(event.key) << '\n';
    }

    void on_key_released(const bb::KeyReleasedEvent& event) {
        std::cout << "KeyReleased: " << static_cast<int>(event.key) << '\n';
    }

    void on_mouse_moved(const bb::MouseMovedEvent& event) {
        std::cout << "MouseMoved: " << event.x << ", " << event.y << '\n';
    }

    void on_mouse_button_pressed(const bb::MouseButtonPressedEvent& event) {
        std::cout << "MouseButtonPressed: " << static_cast<int>(event.button) << '\n';
    }

    void on_mouse_button_released(const bb::MouseButtonReleasedEvent& event) {
        std::cout << "MouseButtonReleased: " << static_cast<int>(event.button) << '\n';
    }

    void on_mouse_wheel_scrolled(const bb::MouseWheelScrolledEvent& event) {
        std::cout << "MouseWheelScrolled: " << event.scroll << '\n';
    }
};

int main() {
    bb::ApplicationProperties properties;

    try {
        bb::Application application {properties};
        application.add_scene<MainScene>();
        application.run("main");
    } catch (bb::RuntimeError error) {
        std::cout << "An error occurred: " << error << '\n';
        return 1;
    }

    return 0;
}
