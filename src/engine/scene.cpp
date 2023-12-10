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

    void Scene::capture(const Camera& camera, const glm::vec3& position) {
        application->renderer->capture(camera, position);
    }

    void Scene::add_shader(std::shared_ptr<Shader> shader) {
        application->renderer->add_shader(shader);
    }

    void Scene::add_framebuffer(std::shared_ptr<Framebuffer> framebuffer) {
        application->renderer->add_framebuffer(framebuffer);
    }

    void Scene::add_renderable(const Renderable& renderable) {
        application->renderer->add_renderable(renderable);
    }

    void Scene::add_light(const DirectionalLight& light) {
        application->renderer->add_light(light);
    }

    void Scene::add_light(const PointLight& light) {
        application->renderer->add_light(light);
    }

    void Scene::debug_add_line(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color) {
        application->renderer->debug_add_line(p1, p2, color);
    }

    void Scene::debug_add_lines(const std::vector<glm::vec3>& points, const glm::vec3& color) {
        application->renderer->debug_add_lines(points, color);
    }

    void Scene::debug_add_point(const glm::vec3& p, const glm::vec3& color) {
        application->renderer->debug_add_point(p, color);
    }

    void Scene::debug_add_lamp(const glm::vec3& position, const glm::vec3& color) {
        application->renderer->debug_add_lamp(position, color);
    }
}
