#include <engine/engine.hpp>

struct MyScene : public bb::Scene {
    MyScene()
        : bb::Scene("my_scene") {}

    void on_enter() override {

    }

    void on_exit() override {

    }

    void on_update() override {

    }
};

int main() {
    bb::ApplicationProperties properties;

    try {
        bb::Application application {properties};
        application.add_scene<MyScene>();
        application.run("my_scene");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
