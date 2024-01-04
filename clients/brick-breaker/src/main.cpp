#include "engine/engine.hpp"

#include "level.hpp"
#include "menu.hpp"
#include "data.hpp"

int main() {
    Data data;

    bb::ApplicationProperties properties;
    properties.title = "Brick Breaker";
    properties.user_data = &data;

    try {
        bb::Application application {properties};
        application.add_scene<Menu>();
        application.add_scene<LevelScene>();
        application.run("menu");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
