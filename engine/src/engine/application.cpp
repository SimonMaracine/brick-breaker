#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include "engine/events.hpp"
#include "engine/window.hpp"
#include "engine/application_properties.hpp"
#include "engine/scene.hpp"
#include "engine/application.hpp"
#include "engine/info_and_debug.hpp"
#include "engine/logging.hpp"

namespace bb {
    Application::Application(const ApplicationProperties& properties) {
        WindowProperties window_properties;
        window_properties.width = properties.width;
        window_properties.height = properties.height;
        window_properties.title = properties.title;
        window_properties.fullscreen = properties.fullscreen;
        window_properties.min_width = properties.min_width;
        window_properties.min_height = properties.min_height;

        window = std::make_unique<Window>(window_properties, this);
        renderer = std::make_unique<Renderer>(properties.width, properties.height);

        GlInfoDebug::initialize_debugging();  // TODO only in debug mode

        events.connect<WindowClosedEvent, &Application::on_window_closed>(this);
        events.connect<WindowResizedEvent, &Application::on_window_resized>(this);

        user_data = properties.user_data;

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
        renderer->prerender_setup();

        while (running) {
            dt = calculate_delta();

            window->poll_events();
            events.update();

            current_scene->on_update();

            renderer->render();

            window->refresh();

            check_scene_change();
        }

        renderer->postrender_setup();
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
            renderer->postrender_setup();
            current_scene->on_exit();
            events.disconnect(current_scene);

            current_scene = next_scene;
            next_scene = nullptr;

            current_scene->on_enter();
            renderer->prerender_setup();
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

    float Application::calculate_delta() {
        static constexpr double MAX_DT {1.0 / 20.0};

        static double previous_seconds {Window::get_time()};
        static double total_time {0.0};
        static int frame_count {0};

        const double current_seconds {Window::get_time()};
        const double elapsed_seconds {current_seconds - previous_seconds};
        previous_seconds = current_seconds;

        total_time += elapsed_seconds;

        if (total_time > 0.25) {
            fps = static_cast<double>(frame_count) / total_time;
            frame_count = 0;
            total_time = 0.0;
        }
        frame_count++;

        const double delta_time {std::min(elapsed_seconds, MAX_DT)};

        return static_cast<float>(delta_time);
    }

    void Application::on_window_closed(const WindowClosedEvent&) {
        running = false;
    }

    void Application::on_window_resized(const WindowResizedEvent& event) {
        renderer->resize_framebuffers(event.width, event.height);
    }
}
