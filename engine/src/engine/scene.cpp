#include <string>
#include <utility>

#include "engine/events.hpp"
#include "engine/scene.hpp"
#include "engine/application.hpp"
#include "engine/audio.hpp"

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

    void Scene::set_vsync(bool enabled) {
        application->window->set_vsync(enabled);
    }

    void Scene::capture(const Camera& camera, const glm::vec3& position) {
        application->renderer->capture(camera, position);
    }

    void Scene::capture(const Camera2D& camera_2d) {
        application->renderer->capture(camera_2d);
    }

    void Scene::add_shader(std::shared_ptr<Shader> shader) {
        application->renderer->add_shader(shader);
    }

    void Scene::add_framebuffer(std::shared_ptr<Framebuffer> framebuffer) {
        application->renderer->add_framebuffer(framebuffer);
    }

    void Scene::shadows(
        float left,
        float right,
        float bottom,
        float top,
        float lens_near,
        float lens_far,
        glm::vec3 position
    ) {
        application->renderer->shadows(left, right, bottom, top, lens_near, lens_far, position);
    }

    void Scene::skybox(std::shared_ptr<TextureCubemap> texture) {
        application->renderer->skybox(texture);
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

    void Scene::add_text(const Text& text) {
        application->renderer->add_text(text);
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

    void Scene::play_sound(std::shared_ptr<SoundData> sound_data) {
        AudioManager::play_sound(sound_data);
    }

    void Scene::pause_sound() {
        AudioManager::pause_sound();
    }

    void Scene::resume_sound() {
        AudioManager::resume_sound();
    }
}
