#include <memory>
#include <string>
#include <vector>
#include <cassert>

#include "engine/events.hpp"
#include "engine/window.hpp"
#include "engine/application_properties.hpp"
#include "engine/scene.hpp"
#include "engine/application.hpp"
#include "logging.hpp"

namespace bb {
    Application::Application(const ApplicationProperties& properties) {
        window = std::make_unique<Window>(
            properties.width,
            properties.height,
            properties.title,
            properties.fullscreen,
            this
        );

        events.connect(EventType::WindowClosed, [this](const Event&) {
            running = false;
        });

        log_message("Initialized application\n");
    }

    Application::~Application() {
        for (auto iter {scenes.rbegin()}; iter != scenes.rend(); iter++) {
            Scene* scene {*iter};
            delete scene;
        }

        log_message("Destroyed application\n");
    }

    void Application::run(const std::string& scene_name) {
        setup_scenes(scene_name);

        current_scene->on_enter();

        while (running) {
            window->poll_events();
            events.update();

            current_scene->on_update();

            // TODO rendering

            window->refresh();

            check_scene_change();
        }

        current_scene->on_exit();
    }

    void Application::setup_scenes(const std::string& scene_name) {
        for (Scene* scene : scenes) {
            scene->application = this;

            if (scene->name == scene_name) {
                current_scene = scene;
            }
        }

        assert(current_scene != nullptr);
    }

    void Application::check_scene_change() {
        if (next_scene != nullptr) {
            current_scene->on_exit();

            current_scene = next_scene;
            next_scene = nullptr;

            current_scene->on_enter();
        }
    }

    void Application::change_scene(const std::string& scene_name) {
        for (Scene* scene : scenes) {
            if (scene->name == scene_name) {
                next_scene = scene;
            }
        }

        assert(next_scene != nullptr);
    }
}
