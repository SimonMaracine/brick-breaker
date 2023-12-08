#include <string>
#include <utility>

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

    int Scene::get_width() const {
        return application->window->get_width();
    }

    int Scene::get_height() const {
        return application->window->get_height();
    }

    float Scene::get_delta() const {
        return application->dt;
    }

    double Scene::get_fps() const {
        return application->fps;
    }
}
