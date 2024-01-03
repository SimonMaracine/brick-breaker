#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "engine/post_processing.hpp"
#include "engine/renderable.hpp"
#include "engine/light.hpp"

namespace bb {
    class Application;
    class Framebuffer;
    class Shader;
    class VertexArray;
    class VertexBuffer;
    class UniformBuffer;
    class Camera;

    class Renderer {
    public:
        Renderer(int width, int height);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void capture(const Camera& camera, const glm::vec3& position);
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

        // 3D API
        void add_renderable(const Renderable& renderable);
        void add_light(const DirectionalLight& light);
        void add_light(const PointLight& light);

        // Debug API
        void debug_add_line(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);
        void debug_add_lines(const std::vector<glm::vec3>& points, const glm::vec3& color);

        void debug_add_point(const glm::vec3& p, const glm::vec3& color);
        void debug_add_lamp(const glm::vec3& position, const glm::vec3& color);
    private:
        void render(int width, int height);
        void prerender_setup();
        void postrender_setup();

        void resize_framebuffers(int width, int height);

        // Render functions
        void screen_quad(unsigned int texture);
        void post_processing();
        void end_rendering();

        // Draw functions
        void draw_renderables();
        void draw_renderable(const Renderable& renderable);

        void draw_renderables_outlined();
        void draw_renderable_outlined(const Renderable& renderable);

        void draw_renderables_to_depth_buffer();

        // Helper functions
        void setup_point_light_uniform_buffer(std::shared_ptr<UniformBuffer> uniform_buffer);
        void setup_light_space_uniform_buffer(std::shared_ptr<UniformBuffer> uniform_buffer);

        struct {
            std::shared_ptr<Framebuffer> scene_framebuffer;
            std::shared_ptr<Framebuffer> shadow_map_framebuffer;

            std::unique_ptr<Shader> screen_quad_shader;
            std::shared_ptr<Shader> shadow_shader;

            std::unique_ptr<VertexArray> screen_quad_vertex_array;

            std::unordered_map<unsigned int, std::weak_ptr<UniformBuffer>> uniform_buffers;
            std::weak_ptr<UniformBuffer> projection_view_uniform_buffer;
            std::weak_ptr<UniformBuffer> directional_light_uniform_buffer;
            std::weak_ptr<UniformBuffer> view_position_uniform_buffer;
            std::weak_ptr<UniformBuffer> point_light_uniform_buffer;
            std::weak_ptr<UniformBuffer> light_space_uniform_buffer;
        } storage;

        PostProcessingContext post_processing_context;

        struct {
            glm::mat4 view_matrix {glm::mat4(1.0f)};
            glm::mat4 projection_matrix {glm::mat4(1.0f)};
            glm::mat4 projection_view_matrix {glm::mat4(1.0f)};
            glm::vec3 position {};
        } camera;

        struct SceneList {
            std::vector<Renderable> renderables;
            DirectionalLight directional_light;
            std::vector<PointLight> point_lights;

            struct LightSpace {
                float left {0.0f};
                float right {0.0f};
                float bottom {0.0f};
                float top {0.0f};
                float lens_near {1.0f};
                float lens_far {1.0f};
                glm::vec3 position {};
            } light_space;

            void clear();
        } scene_list;

        struct {
            std::vector<std::weak_ptr<Shader>> shaders;
            std::vector<std::weak_ptr<Framebuffer>> framebuffers;
        } scene_data;

        // Debug stuff
        void debug_initialize();
        void debug_render();
        void debug_clear();

        struct {
            std::shared_ptr<Shader> shader;

            std::weak_ptr<VertexBuffer> vertex_buffer;
            std::unique_ptr<VertexArray> vertex_array;
        } debug_storage;

        struct BufferVertexStruct {
            glm::vec3 position;
            glm::vec3 color;
        };

        struct Line {
            glm::vec3 p1;
            glm::vec3 p2;
            glm::vec3 color;
        };

        std::vector<Line> debug_scene_list;

        friend class Application;
    };
}
