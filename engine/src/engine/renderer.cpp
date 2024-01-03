#include <vector>
#include <memory>
#include <unordered_map>
#include <cstddef>
#include <array>
#include <algorithm>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <resmanager/resmanager.hpp>

#include "engine/vertex_array.hpp"
#include "engine/buffer.hpp"
#include "engine/framebuffer.hpp"
#include "engine/shader.hpp"
#include "engine/vertex_buffer_layout.hpp"
#include "engine/renderer.hpp"
#include "engine/material.hpp"
#include "engine/opengl.hpp"
#include "engine/post_processing.hpp"
#include "engine/camera.hpp"
#include "engine/renderable.hpp"
#include "engine/light.hpp"

using namespace resmanager::literals;

namespace bb {
    static constexpr unsigned int PROJECTON_VIEW_UNIFORM_BLOCK_BINDING {0};
    static constexpr unsigned int DIRECTIONAL_LIGHT_UNIFORM_BLOCK_BINDING {1};
    static constexpr unsigned int VIEW_POSITION_BLOCK_BINDING {2};
    static constexpr unsigned int POINT_LIGHT_BLOCK_BINDING {3};
    static constexpr unsigned int LIGHT_SPACE_BLOCK_BINDING {4};
    static constexpr std::size_t SHADER_POINT_LIGHTS {4};
    static constexpr int SHADOW_MAP_UNIT {1};

    Renderer::Renderer(int width, int height) {
        OpenGl::enable_depth_test();
        OpenGl::clear_color(0.0f, 0.0f, 0.0f);

        {
            FramebufferSpecification specification;
            specification.width = width;
            specification.height = height;
            specification.color_attachments = {
                Attachment(AttachmentFormat::Rgba8, AttachmentType::Texture)
            };
            specification.depth_attachment = Attachment(
                AttachmentFormat::Depth32, AttachmentType::Renderbuffer
            );

            storage.scene_framebuffer = std::make_shared<Framebuffer>(specification);

            add_framebuffer(storage.scene_framebuffer);
        }

        {
            FramebufferSpecification specification;
            specification.width = 2048;
            specification.height = 2048;
            specification.depth_attachment = Attachment(AttachmentFormat::Depth32, AttachmentType::Texture);
            specification.white_border_for_depth_texture = true;
            specification.resizable = false;

            storage.shadow_map_framebuffer = std::make_shared<Framebuffer>(specification);

            add_framebuffer(storage.shadow_map_framebuffer);
        }

        {
            // Doesn't have uniform buffers for sure
            storage.screen_quad_shader = std::make_unique<Shader>("data/shaders/screen_quad.vert", "data/shaders/screen_quad.frag");

            const float vertices[] {
                -1.0f,  1.0f,
                -1.0f, -1.0f,
                 1.0f,  1.0f,
                 1.0f,  1.0f,
                -1.0f, -1.0f,
                 1.0f, -1.0f
            };

            auto screen_quad_vertex_buffer {std::make_shared<VertexBuffer>(vertices, sizeof(vertices))};

            VertexBufferLayout layout;
            layout.add(0, VertexBufferLayout::Float, 2);

            storage.screen_quad_vertex_array = std::make_unique<VertexArray>();
            storage.screen_quad_vertex_array->bind();
            storage.screen_quad_vertex_array->add_vertex_buffer(screen_quad_vertex_buffer, layout);
            VertexArray::unbind();
        }

        {
            storage.shadow_shader = std::make_shared<Shader>("data/shaders/shadow.vert", "data/shaders/shadow.frag");

            add_shader(storage.shadow_shader);
        }

        debug_initialize();
    }

    Renderer::~Renderer() {

    }

    void Renderer::capture(const Camera& camera, const glm::vec3& position) {
        this->camera.view_matrix = camera.view_matrix;
        this->camera.projection_matrix = camera.projection_matrix;
        this->camera.projection_view_matrix = camera.projection_view_matrix;
        this->camera.position = position;
    }

    void Renderer::add_shader(std::shared_ptr<Shader> shader) {
        scene_data.shaders.push_back(shader);
    }

    void Renderer::add_framebuffer(std::shared_ptr<Framebuffer> framebuffer) {
        scene_data.framebuffers.push_back(framebuffer);
    }

    void Renderer::shadows(
        float left,
        float right,
        float bottom,
        float top,
        float lens_near,
        float lens_far,
        glm::vec3 position
    ) {
        scene_list.light_space.left = left;
        scene_list.light_space.right = right;
        scene_list.light_space.bottom = bottom;
        scene_list.light_space.top = top;
        scene_list.light_space.lens_near = lens_near;
        scene_list.light_space.lens_far = lens_far;
        scene_list.light_space.position = position;
    }

    void Renderer::add_renderable(const Renderable& renderable) {
        scene_list.renderables.push_back(renderable);
    }

    void Renderer::add_light(const DirectionalLight& light) {
        scene_list.directional_light = light;
    }

    void Renderer::add_light(const PointLight& light) {
        scene_list.point_lights.push_back(light);
    }

    void Renderer::debug_add_line(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color) {
        Line line;
        line.p1 = p1;
        line.p2 = p2;
        line.color = color;

        debug_scene_list.push_back(line);
    }

    void Renderer::debug_add_lines(const std::vector<glm::vec3>& points, const glm::vec3& color) {
        assert(points.size() >= 2);

        Line line;
        line.color = color;

        for (std::size_t i {1}; i < points.size(); i++) {
            line.p1 = points[i - 1];
            line.p2 = points[i];

            debug_scene_list.push_back(line);
        }
    }

    void Renderer::debug_add_point(const glm::vec3& p, const glm::vec3& color) {
        static constexpr float SIZE {0.5f};

        debug_add_line(glm::vec3(-SIZE, 0.0f, 0.0f) + p, glm::vec3(SIZE, 0.0f, 0.0f) + p, color);
        debug_add_line(glm::vec3(0.0f, -SIZE, 0.0f) + p, glm::vec3(0.0f, SIZE, 0.0f) + p, color);
        debug_add_line(glm::vec3(0.0f, 0.0f, -SIZE) + p, glm::vec3(0.0f, 0.0f, SIZE) + p, color);
    }

    void Renderer::debug_add_lamp(const glm::vec3& position, const glm::vec3& color) {
        static constexpr float SIZE {0.3f};
        static constexpr float SIZE2 {0.15f};
        static constexpr float SIZE3 {0.5f};
        static constexpr float OFFSET {-(SIZE + SIZE3)};
        const std::array<Line, 24> LINES {
            // Top
            Line {glm::vec3(SIZE, -SIZE, SIZE), glm::vec3(SIZE, -SIZE, -SIZE), color},
            Line {glm::vec3(SIZE, -SIZE, SIZE), glm::vec3(SIZE, SIZE, SIZE), color},
            Line {glm::vec3(SIZE, -SIZE, SIZE), glm::vec3(-SIZE, -SIZE, SIZE), color},

            Line {glm::vec3(-SIZE, -SIZE, -SIZE), glm::vec3(-SIZE, -SIZE, SIZE), color},
            Line {glm::vec3(-SIZE, -SIZE, -SIZE), glm::vec3(-SIZE, SIZE, -SIZE), color},
            Line {glm::vec3(-SIZE, -SIZE, -SIZE), glm::vec3(SIZE, -SIZE, -SIZE), color},

            Line {glm::vec3(SIZE, -SIZE, -SIZE), glm::vec3(SIZE, SIZE, -SIZE), color},
            Line {glm::vec3(-SIZE, -SIZE, SIZE), glm::vec3(-SIZE, SIZE, SIZE), color},

            Line {glm::vec3(SIZE, SIZE, SIZE), glm::vec3(SIZE, SIZE, -SIZE), color},
            Line {glm::vec3(SIZE, SIZE, SIZE), glm::vec3(-SIZE, SIZE, SIZE), color},
            Line {glm::vec3(-SIZE, SIZE, -SIZE), glm::vec3(SIZE, SIZE, -SIZE), color},
            Line {glm::vec3(-SIZE, SIZE, -SIZE), glm::vec3(-SIZE, SIZE, SIZE), color},

            // Bottom
            Line {glm::vec3(SIZE2, -SIZE3 + OFFSET, SIZE2), glm::vec3(SIZE2, -SIZE3 + OFFSET, -SIZE2), color},
            Line {glm::vec3(SIZE2, -SIZE3 + OFFSET, SIZE2), glm::vec3(SIZE2, SIZE3 + OFFSET, SIZE2), color},
            Line {glm::vec3(SIZE2, -SIZE3 + OFFSET, SIZE2), glm::vec3(-SIZE2, -SIZE3 + OFFSET, SIZE2), color},

            Line {glm::vec3(-SIZE2, -SIZE3 + OFFSET, -SIZE2), glm::vec3(-SIZE2, -SIZE3 + OFFSET, SIZE2), color},
            Line {glm::vec3(-SIZE2, -SIZE3 + OFFSET, -SIZE2), glm::vec3(-SIZE2, SIZE3 + OFFSET, -SIZE2), color},
            Line {glm::vec3(-SIZE2, -SIZE3 + OFFSET, -SIZE2), glm::vec3(SIZE2, -SIZE3 + OFFSET, -SIZE2), color},

            Line {glm::vec3(SIZE2, -SIZE3 + OFFSET, -SIZE2), glm::vec3(SIZE2, SIZE3 + OFFSET, -SIZE2), color},
            Line {glm::vec3(-SIZE2, -SIZE3 + OFFSET, SIZE2), glm::vec3(-SIZE2, SIZE3 + OFFSET, SIZE2), color},

            Line {glm::vec3(SIZE2, SIZE3 + OFFSET, SIZE2), glm::vec3(SIZE2, SIZE3 + OFFSET, -SIZE2), color},
            Line {glm::vec3(SIZE2, SIZE3 + OFFSET, SIZE2), glm::vec3(-SIZE2, SIZE3 + OFFSET, SIZE2), color},
            Line {glm::vec3(-SIZE2, SIZE3 + OFFSET, -SIZE2), glm::vec3(SIZE2, SIZE3 + OFFSET, -SIZE2), color},
            Line {glm::vec3(-SIZE2, SIZE3 + OFFSET, -SIZE2), glm::vec3(-SIZE2, SIZE3 + OFFSET, SIZE2), color},
        };

        for (const Line& line : LINES) {
            debug_add_line(line.p1 + position, line.p2 + position, line.color);
        }
    }

    void Renderer::render(int width, int height) {
        // TODO pre-render setup

        {
            auto uniform_buffer {storage.projection_view_uniform_buffer.lock()};

            if (uniform_buffer != nullptr) {
                uniform_buffer->set(&camera.projection_view_matrix, "u_projection_view_matrix"_H);
            }
        }
        {
            auto uniform_buffer {storage.directional_light_uniform_buffer.lock()};

            if (uniform_buffer != nullptr) {
                uniform_buffer->set(&scene_list.directional_light.direction, "u_directional_light.direction"_H);
                uniform_buffer->set(&scene_list.directional_light.ambient_color, "u_directional_light.ambient"_H);
                uniform_buffer->set(&scene_list.directional_light.diffuse_color, "u_directional_light.diffuse"_H);
                uniform_buffer->set(&scene_list.directional_light.specular_color, "u_directional_light.specular"_H);
            }
        }
        {
            auto uniform_buffer {storage.view_position_uniform_buffer.lock()};

            if (uniform_buffer != nullptr) {
                uniform_buffer->set(&camera.position, "u_view_position"_H);
            }
        }
        {
            auto uniform_buffer {storage.point_light_uniform_buffer.lock()};

            if (uniform_buffer != nullptr) {
                setup_point_light_uniform_buffer(uniform_buffer);
            }
        }
        {
            auto uniform_buffer {storage.light_space_uniform_buffer.lock()};

            if (uniform_buffer != nullptr) {
                setup_light_space_uniform_buffer(uniform_buffer);
            }
        }

        for (const auto& [_, wuniform_buffer] : storage.uniform_buffers) {
            std::shared_ptr<UniformBuffer> uniform_buffer {wuniform_buffer.lock()};

            if (uniform_buffer == nullptr) {
                continue;
            }

            uniform_buffer->bind();
            uniform_buffer->upload();
        }

        UniformBuffer::unbind();

        // TODO draw to depth buffer for shadows

        storage.shadow_map_framebuffer->bind();

        OpenGl::clear(OpenGl::Buffers::D);
        OpenGl::viewport(
            storage.shadow_map_framebuffer->get_specification().width,
            storage.shadow_map_framebuffer->get_specification().height
        );

        draw_renderables_to_depth_buffer();

        storage.scene_framebuffer->bind();

        OpenGl::clear(OpenGl::Buffers::CD);
        OpenGl::viewport(
            storage.scene_framebuffer->get_specification().width,
            storage.scene_framebuffer->get_specification().height
        );

        OpenGl::bind_texture_2d(storage.shadow_map_framebuffer->get_depth_attachment(), SHADOW_MAP_UNIT);

        draw_renderables();
        draw_renderables_outlined();

        // Do post processing and render the final 3D image to the screen
        end_rendering();

        scene_list.clear();

        debug_render();
        debug_clear();
    }

    void Renderer::prerender_setup() {
        for (const std::weak_ptr<Shader>& wshader : scene_data.shaders) {
            std::shared_ptr<Shader> shader {wshader.lock()};

            if (shader == nullptr) {
                continue;
            }

            // Create and store references to particular uniform buffers
            for (const UniformBlockSpecification& block : shader->uniform_blocks) {
                // Don't create duplicate buffers
                if (storage.uniform_buffers.count(block.binding_index) == 1) {
                    continue;
                }

                auto uniform_buffer {std::make_shared<UniformBuffer>(block)};
                shader->add_uniform_buffer(uniform_buffer);

                storage.uniform_buffers[block.binding_index] = uniform_buffer;

                switch (block.binding_index) {
                    case PROJECTON_VIEW_UNIFORM_BLOCK_BINDING:
                        storage.projection_view_uniform_buffer = uniform_buffer;
                        break;
                    case DIRECTIONAL_LIGHT_UNIFORM_BLOCK_BINDING:
                        storage.directional_light_uniform_buffer = uniform_buffer;
                        break;
                    case VIEW_POSITION_BLOCK_BINDING:
                        storage.view_position_uniform_buffer = uniform_buffer;
                        break;
                    case POINT_LIGHT_BLOCK_BINDING:
                        storage.point_light_uniform_buffer = uniform_buffer;
                        break;
                    case LIGHT_SPACE_BLOCK_BINDING:
                        storage.light_space_uniform_buffer = uniform_buffer;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void Renderer::postrender_setup() {
        scene_data.shaders.clear();
    }

    void Renderer::resize_framebuffers(int width, int height) {
        if (width == 0 || height == 0) {
            return;
        }

        OpenGl::viewport(width, height);

        for (std::weak_ptr<Framebuffer> wframebuffer : scene_data.framebuffers) {
            std::shared_ptr<Framebuffer> framebuffer {wframebuffer.lock()};

            if (framebuffer == nullptr) {
                continue;
            }

            if (!framebuffer->get_specification().resizable) {
                continue;
            }

            framebuffer->resize(width, height);
        }
    }

    void Renderer::screen_quad(unsigned int texture) {
        storage.screen_quad_shader->bind();
        OpenGl::bind_texture_2d(texture, 0);
        OpenGl::draw_arrays(6);
    }

    void Renderer::post_processing() {

    }

    void Renderer::end_rendering() {
        storage.screen_quad_vertex_array->bind();

        OpenGl::disable_depth_test();

        post_processing();

        // Draw the final result to the screen
        Framebuffer::bind_default();

        // Clear even the default framebuffer, for debug renderer
        OpenGl::clear(OpenGl::Buffers::CD);

        // screen_quad(post_processing_context.last_texture);  // FIXME
        screen_quad(storage.scene_framebuffer->get_color_attachment(0));

        OpenGl::enable_depth_test();

        VertexArray::unbind();
    }

    void Renderer::draw_renderables() {
        for (const Renderable& renderable : scene_list.renderables) {
            auto material {renderable.material.lock()};

            if (material->flags & Material::Outline) {
                continue;  // This one is rendered differently
            }

            if (material->flags & Material::DisableBackFaceCulling) {
                OpenGl::disable_back_face_culling();
            }

            draw_renderable(renderable);

            if (material->flags & Material::DisableBackFaceCulling) {
                OpenGl::enable_back_face_culling();
            }
        }

        VertexArray::unbind();  // Don't unbind for every renderable
    }

    void Renderer::draw_renderable(const Renderable& renderable) {
        auto vertex_array {renderable.vertex_array.lock()};
        auto material {renderable.material.lock()};

        glm::mat4 matrix;

        if (renderable.transformation) {
            matrix = *renderable.transformation;
        } else {
            matrix = glm::mat4(1.0f);
            matrix = glm::translate(matrix, renderable.position);
            matrix = glm::rotate(matrix, renderable.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            matrix = glm::rotate(matrix, renderable.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            matrix = glm::rotate(matrix, renderable.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            matrix = glm::scale(matrix, glm::vec3(renderable.scale));
        }

        vertex_array->bind();
        material->bind_and_upload();

        material->get_shader()->upload_uniform_mat4("u_model_matrix"_H, matrix);

        OpenGl::draw_elements(vertex_array->get_index_buffer()->get_index_count());
    }

    void Renderer::draw_renderables_outlined() {

    }

    void Renderer::draw_renderable_outlined(const Renderable& renderable) {

    }

    void Renderer::draw_renderables_to_depth_buffer() {
        storage.shadow_shader->bind();

        for (const Renderable& renderable : scene_list.renderables) {
            auto vertex_array {renderable.vertex_array.lock()};
            auto material {renderable.material.lock()};

            if (material->flags & Material::CastShadow) {
                glm::mat4 matrix;

                if (renderable.transformation) {
                    matrix = *renderable.transformation;
                } else {
                    matrix = glm::mat4(1.0f);
                    matrix = glm::translate(matrix, renderable.position);
                    matrix = glm::rotate(matrix, renderable.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
                    matrix = glm::rotate(matrix, renderable.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
                    matrix = glm::rotate(matrix, renderable.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
                    matrix = glm::scale(matrix, glm::vec3(renderable.scale));
                }

                storage.shadow_shader->upload_uniform_mat4("u_model_matrix"_H, matrix);

                vertex_array->bind();

                OpenGl::draw_elements(vertex_array->get_index_buffer()->get_index_count());
            }
        }

        // Don't unbind for every model
        VertexArray::unbind();
    }

    void Renderer::setup_point_light_uniform_buffer(const std::shared_ptr<UniformBuffer> uniform_buffer) {
        // Sort front to back with respect to the camera; lights in the front of the list will be used
        std::sort(
            scene_list.point_lights.begin(),
            scene_list.point_lights.end(),
            [this](const PointLight& lhs, const PointLight& rhs) {
                const float distance_left {glm::distance(lhs.position, camera.position)};
                const float distance_right {glm::distance(rhs.position, camera.position)};

                return distance_left < distance_right;
            }
        );

        // Add dummy point lights to make the size 4, which is a requirement from the shader
        if (scene_list.point_lights.size() < SHADER_POINT_LIGHTS) {
            scene_list.point_lights.resize(SHADER_POINT_LIGHTS);
        }

        for (std::size_t i {0}; i < SHADER_POINT_LIGHTS; i++) {
            const PointLight& light {scene_list.point_lights[i]};
            const std::string index {std::to_string(i)};

            uniform_buffer->set(&light.position, resmanager::HashedStr64("u_point_lights[" + index + "].position"));
            uniform_buffer->set(&light.ambient_color, resmanager::HashedStr64("u_point_lights[" + index + "].ambient"));
            uniform_buffer->set(&light.diffuse_color, resmanager::HashedStr64("u_point_lights[" + index + "].diffuse"));
            uniform_buffer->set(&light.specular_color, resmanager::HashedStr64("u_point_lights[" + index + "].specular"));
            uniform_buffer->set(&light.falloff_linear, resmanager::HashedStr64("u_point_lights[" + index + "].falloff_linear"));
            uniform_buffer->set(&light.falloff_quadratic, resmanager::HashedStr64("u_point_lights[" + index + "].falloff_quadratic"));
        }
    }

    void Renderer::setup_light_space_uniform_buffer(std::shared_ptr<UniformBuffer> uniform_buffer) {
        const glm::mat4 projection = glm::ortho(
            scene_list.light_space.left,
            scene_list.light_space.right,
            scene_list.light_space.bottom,
            scene_list.light_space.top,
            scene_list.light_space.lens_near,
            scene_list.light_space.lens_far
        );

        const glm::mat4 view = glm::lookAt(
            scene_list.light_space.position,
            scene_list.directional_light.direction,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        const glm::mat4 light_space_matrix = projection * view;

        uniform_buffer->set(&light_space_matrix, "u_light_space_matrix"_H);
    }

    void Renderer::SceneList::clear() {
        renderables.clear();
        directional_light = {};
        point_lights.clear();
    }

    void Renderer::debug_initialize() {
        debug_storage.shader = std::make_shared<Shader>("data/shaders/debug.vert", "data/shaders/debug.frag");

        add_shader(debug_storage.shader);

        auto vertex_buffer {std::make_shared<VertexBuffer>(DrawHint::Stream)};
        debug_storage.vertex_buffer = vertex_buffer;

        VertexBufferLayout layout;
        layout.add(0, VertexBufferLayout::Float, 3);
        layout.add(1, VertexBufferLayout::Float, 3);

        debug_storage.vertex_array = std::make_unique<VertexArray>();
        debug_storage.vertex_array->bind();
        debug_storage.vertex_array->add_vertex_buffer(vertex_buffer, layout);
        VertexArray::unbind();
    }

    void Renderer::debug_render() {
        static std::vector<BufferVertexStruct> buffer;
        buffer.clear();

        for (const Line& line : debug_scene_list) {
            BufferVertexStruct v1;
            v1.position = line.p1;
            v1.color = line.color;

            buffer.push_back(v1);

            BufferVertexStruct v2;
            v2.position = line.p2;
            v2.color = line.color;

            buffer.push_back(v2);
        }

        if (buffer.empty()) {
            return;
        }

        auto vertex_buffer {debug_storage.vertex_buffer.lock()};

        vertex_buffer->bind();
        vertex_buffer->upload_data(buffer.data(), buffer.size() * sizeof(BufferVertexStruct));
        VertexBuffer::unbind();

        debug_storage.shader->bind();
        debug_storage.vertex_array->bind();

        OpenGl::draw_arrays_lines(debug_scene_list.size() * 2);

        VertexArray::unbind();

        debug_scene_list.clear();
    }

    void Renderer::debug_clear() {
        debug_scene_list.clear();
    }
}
