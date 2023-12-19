#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstddef>
#include <utility>
#include <memory>

#include <glm/glm.hpp>
#include <resmanager/resmanager.hpp>

#include "engine/buffer.hpp"

namespace bb {
    class Renderer;

    class Shader {
    public:
        using Key = resmanager::HashedStr64;
        using KeyHash = resmanager::Hash<Key>;

        Shader(const std::string& source_vertex, const std::string& source_fragment);
        Shader(const std::string& source_vertex, const std::string& source_fragment, const std::string& includes);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(Shader&&) = delete;

        void bind() const;
        static void unbind();

        void upload_uniform_mat4(Key name, const glm::mat4& matrix) const;
        void upload_uniform_int(Key name, int value) const;
        void upload_uniform_float(Key name, float value) const;
        void upload_uniform_vec2(Key name, glm::vec2 vector) const;
        void upload_uniform_vec3(Key name, const glm::vec3& vector) const;
        void upload_uniform_vec4(Key name, const glm::vec4& vector) const;

        unsigned int get_id() const { return program; }

        void add_uniform_buffer(std::shared_ptr<UniformBuffer> uniform_buffer);
    private:
        int get_uniform_location(Key name) const;
        void check_and_cache_uniforms();

        void introspect_program();

        unsigned int create_program() const;
        void delete_intermediates();
        unsigned int compile_shader(const std::string& source_path, unsigned int type) const;
        unsigned int compile_shader(const std::pair<unsigned char*, std::size_t>& source_buffer, unsigned int type) const;
        bool check_compilation(unsigned int shader, unsigned int type) const;
        bool check_linking(unsigned int program) const;

        unsigned int program {0};
        unsigned int vertex_shader {0};
        unsigned int fragment_shader {0};

        // Uniforms cache
        std::unordered_map<Key, int, KeyHash> cache;

        // Data from introspection
        std::vector<std::string> uniforms;
        std::vector<UniformBlockSpecification> uniform_blocks;

        // Shaders own uniform buffers
        std::vector<std::shared_ptr<UniformBuffer>> uniform_buffers;

        friend class Renderer;
    };
}
