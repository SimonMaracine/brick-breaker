#pragma once

#include <string>
#include <utility>

#include "engine/application.hpp"
#include "engine/events.hpp"

namespace bb {
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

        template<typename E, typename... Args>
        void enqueue_event(Args&&... args) {
            application->events.template enqueue<E>(std::forward<Args>(args)...);
        }

        template<typename E, auto F, typename T>
        void connect_event(T&& instance) {
            application->events.template connect<E, F>(std::forward<T>(instance));
        }

        template<typename T>
        void disconnect_event(T&& instance) {
            application->events.template disconnect(std::forward<T>(instance));
        }
    private:
        std::string name;
        Application* application {nullptr};

        friend class Application;
    };
}
