#include <string>

#include "engine/events.hpp"
#include "engine/scene.hpp"
#include "engine/application.hpp"

namespace bb {
    void Scene::change_scene(const std::string& scene_name) {
        application->change_scene(scene_name);
    }

    void Scene::quit_application() {
        application->running = false;
    }

    void Scene::connect_event(EventType type, const EventSystem::EventCallback& callback) {
        application->events.connect(type, callback);
    }

    void Scene::disconnect_event(EventType type) {
        application->events.disconnect(type);
    }

    void Scene::disconnect_event() {
        application->events.disconnect();
    }
}
