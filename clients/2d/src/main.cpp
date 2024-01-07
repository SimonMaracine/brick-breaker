#include <engine/engine.hpp>

#include "main_scene.hpp"

int main() {
    bb::ApplicationProperties properties;
    properties.title = "2D";
    properties.disable_renderer = true;

    try {
        bb::Application application {properties};
        application.add_scene<Main>();
        application.run("main");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
