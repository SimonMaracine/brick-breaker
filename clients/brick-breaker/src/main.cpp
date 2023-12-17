#include "engine/engine.hpp"

#include "level.hpp"

int main() {
    bb::ApplicationProperties properties;
    properties.title = "Brick Breaker";

    try {
        bb::Application application {properties};
        application.add_scene<LevelScene>();
        application.run("level");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
