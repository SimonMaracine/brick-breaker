#include <string>
#include <unordered_map>
#include <vector>
#include <cstddef>
#include <utility>
#include <memory>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <cstdlib>

#include <glm/glm.hpp>
#include <resmanager/resmanager.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_include.h>

#include "engine/panic.hpp"
#include "engine/shader.hpp"
#include "engine/buffer.hpp"
#include "engine/logging.hpp"

namespace bb {
    Shader::Shader(const std::string& source_vertex, const std::string& source_fragment) {
        vertex_shader = compile_shader(source_vertex, GL_VERTEX_SHADER);
        fragment_shader = compile_shader(source_fragment, GL_FRAGMENT_SHADER);
        program = create_program();

        if (!check_linking(program)) {
            log_message("Could not link shader program %d\n", program);
            throw ResourceLoadingError;
        }

        delete_intermediates();
        introspect_program();
        check_and_cache_uniforms();
    }

    Shader::Shader(const std::string& source_vertex, const std::string& source_fragment, const std::string& includes) {
        char error[256] {};
        char* result_vertex {nullptr};
        char* result_fragment {nullptr};

        try {
            result_vertex = stb_include_file(
                const_cast<char*>(source_vertex.c_str()),
                nullptr,
                const_cast<char*>(includes.c_str()),
                error
            );

            if (result_vertex == nullptr) {
                log_message("Could not load the files of shader program %d: %s\n", program, error);
                throw ResourceLoadingError;
            }

            result_fragment = stb_include_file(
                const_cast<char*>(source_fragment.c_str()),
                nullptr,
                const_cast<char*>(includes.c_str()),
                error
            );

            if (result_fragment == nullptr) {
                log_message("Could not load the files of shader program %d: %s\n", program, error);
                throw ResourceLoadingError;
            }

            vertex_shader = compile_shader(
                std::make_pair(reinterpret_cast<unsigned char*>(result_vertex), std::strlen(result_vertex)),
                GL_VERTEX_SHADER
            );
            fragment_shader = compile_shader(
                std::make_pair(reinterpret_cast<unsigned char*>(result_fragment), std::strlen(result_fragment)),
                GL_FRAGMENT_SHADER
            );
            program = create_program();

            if (!check_linking(program)) {
                log_message("Could not link shader program %d\n", program);
                throw ResourceLoadingError;
            }

            delete_intermediates();
            introspect_program();
            check_and_cache_uniforms();
        } catch (RuntimeError) {
            // This makes sure that memory is freed
            std::free(result_vertex);
            std::free(result_fragment);

            throw;
        }
    }

    Shader::~Shader() {
        glDeleteProgram(program);
    }

    void Shader::bind() const {
        glUseProgram(program);
    }

    void Shader::unbind() {
        glUseProgram(0);
    }

    void Shader::upload_uniform_mat4(Key name, const glm::mat4& matrix) const {
        const int location {get_uniform_location(name)};
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::upload_uniform_int(Key name, int value) const {
        const int location {get_uniform_location(name)};
        glUniform1i(location, value);
    }

    void Shader::upload_uniform_float(Key name, float value) const {
        const int location {get_uniform_location(name)};
        glUniform1f(location, value);
    }

    void Shader::upload_uniform_vec2(Key name, glm::vec2 vector) const {
        const int location {get_uniform_location(name)};
        glUniform2f(location, vector.x, vector.y);
    }

    void Shader::upload_uniform_vec3(Key name, const glm::vec3& vector) const {
        const int location {get_uniform_location(name)};
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void Shader::upload_uniform_vec4(Key name, const glm::vec4& vector) const {
        const int location {get_uniform_location(name)};
        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }

    void Shader::add_uniform_buffer(std::shared_ptr<UniformBuffer> uniform_buffer) {
        uniform_buffer->bind();
        uniform_buffer->configure(program);  // No problem, if it's already configured
        UniformBuffer::unbind();

        uniform_buffers.push_back(uniform_buffer);
    }

    int Shader::get_uniform_location(Key name) const {
        return cache.at(name);
    }

    void Shader::check_and_cache_uniforms() {
        for (const auto& uniform : uniforms) {
            int location {0};

            // Don't touch uniforms that are in blocks
            for (const auto& block : uniform_blocks) {
                if (std::find(block.uniforms.cbegin(), block.uniforms.cend(), uniform) != block.uniforms.cend()) {
                    goto skip_uniform;
                }
            }

            location = glGetUniformLocation(program, uniform.c_str());

            if (location == -1) {
                log_message("Uniform variable `%s` not found\n", uniform.c_str());
                continue;
            }

            cache[Key(uniform)] = location;

            skip_uniform:
            continue;
        }
    }

    void Shader::introspect_program() {
        // Uniforms stuff
        int uniform_count;
        glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniform_count);

        uniforms.reserve(uniform_count);

        for (unsigned int i {0}; i < static_cast<unsigned int>(uniform_count); i++) {
            char buffer[64];
            glGetProgramResourceName(program, GL_UNIFORM, i, 64, nullptr, buffer);

            uniforms.push_back(buffer);
        }

        // Uniform blocks stuff
        int uniform_block_count;
        glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniform_block_count);

        uniform_blocks.reserve(uniform_block_count);

        for (unsigned int i {0}; i < static_cast<unsigned int>(uniform_block_count); i++) {
            UniformBlockSpecification block;
            std::size_t block_active_uniforms_count {0};

            {
                char buffer[64];
                glGetProgramResourceName(program, GL_UNIFORM_BLOCK, i, 64, nullptr, buffer);

                block.block_name = buffer;
            }

            {
                int buffer[2];
                const GLenum properties[] { GL_BUFFER_BINDING, GL_NUM_ACTIVE_VARIABLES };
                glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, i, 2, properties, 2, nullptr, buffer);

                block.binding_index = static_cast<unsigned int>(buffer[0]);
                block_active_uniforms_count = static_cast<std::size_t>(buffer[1]);
                block.uniforms.reserve(block_active_uniforms_count);
            }

            {
                int* buffer_uniforms {new int[block_active_uniforms_count]};
                const GLenum properties[] { GL_ACTIVE_VARIABLES };
                glGetProgramResourceiv(
                    program, GL_UNIFORM_BLOCK, i, 1, properties,
                    block_active_uniforms_count, nullptr, buffer_uniforms
                );

                for (unsigned int j {0}; j < static_cast<unsigned int>(block_active_uniforms_count); j++) {
                    char buffer[64];
                    glGetProgramResourceName(program, GL_UNIFORM, buffer_uniforms[j], 64, nullptr, buffer);

                    block.uniforms.push_back(buffer);
                }

                delete[] buffer_uniforms;
            }

            uniform_blocks.push_back(block);
        }
    }

    unsigned int Shader::create_program() const {
        assert(vertex_shader != 0);
        assert(fragment_shader != 0);

        const unsigned int program {glCreateProgram()};
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        glValidateProgram(program);

        return program;
    }

    void Shader::delete_intermediates() {
        assert(program != 0);
        assert(vertex_shader != 0);
        assert(fragment_shader != 0);

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        vertex_shader = 0;
        fragment_shader = 0;
    }

    unsigned int Shader::compile_shader(const std::string& source_path, unsigned int type) const {
        std::ifstream file {source_path, std::ios::binary};

        if (!file.is_open()) {
            log_message("Could not open file `%s` for reading\n", source_path.c_str());
            throw ResourceLoadingError;
        }

        file.seekg(0, file.end);
        const auto length {file.tellg()};
        file.seekg(0, file.beg);

        char* buffer {new char[length]};
        file.read(buffer, length);

        const unsigned int shader {glCreateShader(type)};

        const char* const source {buffer};
        const int source_length {static_cast<int>(length)};

        glShaderSource(shader, 1, &source, &source_length);
        glCompileShader(shader);

        delete[] buffer;

        if (!check_compilation(shader, type)) {
            log_message("Could not compile shader %d\n", shader);
            throw ResourceLoadingError;
        }

        return shader;
    }

    unsigned int Shader::compile_shader(const std::pair<unsigned char*, std::size_t>& source_buffer, unsigned int type) const {
        const unsigned int shader {glCreateShader(type)};

        const char* buffer {reinterpret_cast<const char*>(source_buffer.first)};
        const char* const source {buffer};
        const int source_length {static_cast<int>(source_buffer.second)};

        glShaderSource(shader, 1, &source, &source_length);
        glCompileShader(shader);

        if (!check_compilation(shader, type)) {
            log_message("Could not compile shader %d\n", shader);
            throw ResourceLoadingError;
        }

        return shader;
    }

    bool Shader::check_compilation(unsigned int shader, unsigned int type) const {
        int compile_status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

        if (compile_status == GL_FALSE) {
            int log_length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

            const char* type_name {nullptr};
            switch (type) {
                case GL_VERTEX_SHADER:
                    type_name = "Vertex";
                    break;
                case GL_FRAGMENT_SHADER:
                    type_name = "Fragment";
                    break;
                default:
                    type_name = "Unknown";
                    break;
            }

            if (log_length == 0) {
                log_message("%s shader compilation error with no message\n", type_name);
            } else {
                char* message {new char[log_length]};
                glGetShaderInfoLog(shader, log_length, nullptr, message);

                log_message("%s shader compilation error\n%s\n", type_name, message);
                delete[] message;
            }

            return false;
        }

        return true;
    }

    bool Shader::check_linking(unsigned int program) const {
        int link_status;
        glGetProgramiv(program, GL_LINK_STATUS, &link_status);

        if (link_status == GL_FALSE) {
            int log_length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

            if (log_length == 0) {
                log_message("Linking error with no message\n");
            } else {
                char* message {new char[log_length]};
                glGetProgramInfoLog(program, log_length, nullptr, message);

                log_message("Linking error\n%s\n", message);
                delete[] message;
            }

            return false;
        }

        return true;
    }
}
