#include "engine/engine.hpp"

#include "level.hpp"
#include "menu.hpp"
#include "levels.hpp"
#include "about.hpp"
#include "data.hpp"

int main() {
    Data data;

    bb::ApplicationProperties properties;
    properties.title = "Brick Breaker";
    properties.min_width = 896;
    properties.min_height = 504;
    properties.samples = 4;
    properties.user_data = &data;

    try {
        bb::Application application {properties};
        application.add_scene<MenuScene>();
        application.add_scene<LevelScene>();
        application.add_scene<LevelsScene>();
        application.add_scene<AboutScene>();
        application.run("menu");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
