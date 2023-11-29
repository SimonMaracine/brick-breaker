#pragma once

#include <string>

#include "engine/events.hpp"

namespace bb {
    class Application;

    class Scene {
    public:
        Scene(const std::string& name)
            : name(name) {}

        virtual ~Scene() = default;

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(Scene&&) = delete;

        virtual void on_enter() {}
        virtual void on_exit() {}
        virtual void on_update() {}

        const std::string& get_name() const { return name; }
    protected:
        void change_scene(const std::string& scene_name);
        void quit_application();

        void connect_event(EventType type, const EventSystem::EventCallback& callback);
        void disconnect_event(EventType type);
        void disconnect_event();
    private:
        std::string name;
        Application* application {nullptr};

        friend class Application;
    };
}
