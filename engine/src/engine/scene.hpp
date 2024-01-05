#pragma once

#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include "engine/application.hpp"
#include "engine/events.hpp"
#include "engine/sound_data.hpp"

namespace bb {
    struct Camera;
    struct Camera2D;
    class Shader;
    class Framebuffer;
    struct Renderable;
    struct DirectionalLight;
    struct PointLight;
    class Shader;
    class TextureCubemap;

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
        int get_width() const;
        int get_height() const;
        float get_delta() const;
        double get_fps() const;
        void set_vsync(bool enabled);

        template<typename T>
        T& user_data() {
            return *static_cast<T*>(application->user_data);
        }

        template<typename E, typename... Args>
        void enqueue_event(Args&&... args) {
            application->events.template enqueue<E>(std::forward<Args>(args)...);
        }

        template<typename E, auto F, typename... T>
        void connect_event(T&&... value_or_instance) {
            application->events.template connect<E, F>(value_or_instance...);
        }

        template<typename E, auto F, typename... T>
        void disconnect_event(T&&... value_or_instance) {
            application->events.template disconnect<E, F>(value_or_instance...);
        }

        template<typename T>
        void disconnect_event(T& value_or_instance) {
            application->events.template disconnect(value_or_instance);
        }

        template<typename T>
        void disconnect_event(T* value_or_instance) {
            application->events.template disconnect(value_or_instance);
        }

        void capture(const Camera& camera, const glm::vec3& position);
        void capture(const Camera2D& camera_2d);
        void add_shader(std::shared_ptr<Shader> shader);
        void add_framebuffer(std::shared_ptr<Framebuffer> framebuffer);
        void shadows(
            float left,
            float right,
            float bottom,
            float top,
            float lens_near,
            float lens_far,
            glm::vec3 position
        );
        void skybox(std::shared_ptr<TextureCubemap> texture);

        void add_renderable(const Renderable& renderable);
        void add_light(const DirectionalLight& light);
        void add_light(const PointLight& light);

        void add_text(const Text& text);

        void debug_add_line(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);
        void debug_add_lines(const std::vector<glm::vec3>& points, const glm::vec3& color);
        void debug_add_point(const glm::vec3& p, const glm::vec3& color);
        void debug_add_lamp(const glm::vec3& position, const glm::vec3& color);

        void play_sound(std::shared_ptr<SoundData> sound_data);
        void pause_sound();
        void resume_sound();
    private:
        std::string name;
        Application* application {nullptr};

        friend class Application;
    };
}
