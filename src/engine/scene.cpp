#include <string>

#include "engine/scene.hpp"
#include "engine/application.hpp"

namespace bb {
    void Scene::change_scene(const std::string& scene_name) {
        application->change_scene(scene_name);
    }
}
