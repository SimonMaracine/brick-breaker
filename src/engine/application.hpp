#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/events.hpp"
#include "engine/window.hpp"
#include "engine/application_properties.hpp"
#include "engine/scene.hpp"

namespace bb {
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

        EventSystem events;
        std::unique_ptr<Window> window;

        bool running {true};

        std::vector<Scene*> scenes;
        Scene* current_scene {nullptr};
        Scene* next_scene {nullptr};

        friend class Window;
        friend class Scene;
    };
}
