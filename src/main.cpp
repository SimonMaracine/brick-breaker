#include <iostream>

#include "engine/engine.hpp"

struct MainScene : public bb::Scene {
    MainScene()
        : bb::Scene("main") {}

    virtual void on_enter() override {
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
