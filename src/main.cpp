#include <iostream>

#include "engine/engine.hpp"

struct MainScene : public bb::Scene {
    MainScene()
        : bb::Scene("main") {}

    virtual void on_enter() override {
        connect_event(bb::EventType::KeyPressed, [](const bb::Event& event) {
            std::cout << "KeyPressed: " << static_cast<int>(event.kp.key) << '\n';
        });

        connect_event(bb::EventType::KeyReleased, [](const bb::Event& event) {
            std::cout << "KeyReleased: " << static_cast<int>(event.kr.key) << '\n';
        });

        connect_event(bb::EventType::MouseMoved, [](const bb::Event& event) {
            std::cout << "MouseMoved: " << event.mm.x << ", " << event.mm.y << '\n';
        });

        connect_event(bb::EventType::MouseButtonPressed, [](const bb::Event& event) {
            std::cout << "MouseButtonPressed: " << static_cast<int>(event.mbp.button) << '\n';
        });

        connect_event(bb::EventType::MouseButtonReleased, [](const bb::Event& event) {
            std::cout << "MouseButtonReleased: " << static_cast<int>(event.mbr.button) << '\n';
        });

        connect_event(bb::EventType::MouseWheelScrolled, [](const bb::Event& event) {
            std::cout << "MouseWheelScrolled: " << event.mws.scroll << '\n';
        });

        bb::log_message("Entered main scene\n");
    }

    virtual void on_exit() override {
        bb::log_message("Exited main scene\n");
    }

    virtual void on_update() override {

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
