#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/events.hpp"
#include "engine/window.hpp"
#include "engine/application_properties.hpp"
#include "engine/renderer.hpp"

namespace bb {
    class Scene;

    class Application final {
    public:
        Application(const ApplicationProperties& properties);
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        template<typename S>
        void add_scene() {
            scenes.push_back(new S);
        }

        void run(const std::string& scene_name);
    private:
        void setup_scenes(const std::string& scene_name);
        void check_scene_change();
        void change_scene(const std::string& scene_name);

        void on_window_closed(const WindowClosedEvent&);
        void on_window_resized(const WindowResizedEvent& event);

        EventSystem events;
        std::unique_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;

        bool running {true};

        std::vector<Scene*> scenes;
        Scene* current_scene {nullptr};
        Scene* next_scene {nullptr};

        friend class Window;
        friend class Scene;
    };
}
