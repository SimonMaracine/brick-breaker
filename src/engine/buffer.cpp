#include <cstddef>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>

#include <resmanager/resmanager.hpp>
#include <glad/glad.h>

#include "engine/buffer.hpp"

namespace bb {
    static int draw_hint_to_int(DrawHint hint) {
        int result {0};

        switch (hint) {
            case DrawHint::Static:
                result = GL_STATIC_DRAW;
                break;
            case DrawHint::Dynamic:
                result = GL_DYNAMIC_DRAW;
                break;
            case DrawHint::Stream:
                result = GL_STREAM_DRAW;
                break;
        }

        return result;
    }

    VertexBuffer::VertexBuffer(DrawHint hint)
        : hint(hint) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VertexBuffer::VertexBuffer(std::size_t size, DrawHint hint)
        : hint(hint) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, draw_hint_to_int(hint));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VertexBuffer::VertexBuffer(const void* data, std::size_t size, DrawHint hint)
        : hint(hint) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, data, draw_hint_to_int(hint));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &buffer);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }

    void VertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::upload_data(const void* data, std::size_t size) const {
        glBufferData(GL_ARRAY_BUFFER, size, data, draw_hint_to_int(hint));
    }

    void VertexBuffer::upload_sub_data(const void* data, std::size_t offset, std::size_t size) const {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    IndexBuffer::IndexBuffer(const void* data, std::size_t size) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        assert(size % sizeof(unsigned int) == 0);

        index_count = static_cast<int>(size / sizeof(unsigned int));
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &buffer);
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    }

    void IndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    UniformBuffer::UniformBuffer(const UniformBlockSpecification& specification)
        : specification(specification) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    UniformBuffer::~UniformBuffer() {
        glDeleteBuffers(1, &buffer);

        delete[] data;
    }

    void UniformBuffer::bind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    }

    void UniformBuffer::unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBuffer::configure(unsigned int shader_program) {
        const GLuint block_index {
            glGetUniformBlockIndex(shader_program, specification.block_name.c_str())
        };

        assert(block_index != GL_INVALID_INDEX);

        // If it's already configured, return
        if (configured) {
            return;
        }

        // Get data block size
        GLint block_size;
        glGetActiveUniformBlockiv(shader_program, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

        // Allocate memory on both CPU and GPU side
        allocate_memory(block_size);

        // Link uniform buffer to binding index
        glBindBufferBase(GL_UNIFORM_BUFFER, specification.binding_index, buffer);

        const std::size_t field_count {specification.uniforms.size()};
        static constexpr std::size_t MAX_FIELD_COUNT {24};

        assert(field_count <= MAX_FIELD_COUNT);

        GLuint indices[MAX_FIELD_COUNT];
        GLint offsets[MAX_FIELD_COUNT];
        GLint sizes[MAX_FIELD_COUNT];
        GLint types[MAX_FIELD_COUNT];

        // Create the uniforms names list; the order of these names matters
        const char* field_names[MAX_FIELD_COUNT];

        for (std::size_t i {0}; i < field_count; i++) {
            field_names[i] = specification.uniforms[i].c_str();
        }

        // Get uniform indices just to later get offsets, sizes and types
        glGetUniformIndices(
            shader_program,
            field_count,
            static_cast<const char* const*>(field_names),
            indices
        );

        for (std::size_t i {0}; i < field_count; i++) {
            assert(indices[i] != GL_INVALID_INDEX);
        }

        glGetActiveUniformsiv(shader_program, field_count, indices, GL_UNIFORM_OFFSET, offsets);
        glGetActiveUniformsiv(shader_program, field_count, indices, GL_UNIFORM_SIZE, sizes);  // For arrays
        glGetActiveUniformsiv(shader_program, field_count, indices, GL_UNIFORM_TYPE, types);

        // Finally setup the uniform block fields
        for (std::size_t i {0}; i < field_count; i++) {
            UniformBlockField field;
            field.offset = static_cast<std::size_t>(offsets[i]);
            field.size = static_cast<std::size_t>(sizes[i]) * type_size(types[i]);

            fields[Key(field_names[i])] = field;
        }

        configured = true;
    }

    void UniformBuffer::set(const void* field_data, Key field) {
        assert(configured);
        assert(data != nullptr && size > 0);

        std::memcpy(data + fields.at(field).offset, field_data, fields.at(field).size);
    }

    void UniformBuffer::upload() const {
        assert(data != nullptr && size > 0);

        glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
    }

    void UniformBuffer::set_and_upload(const void* field_data, Key field) {
        assert(configured);
        assert(data != nullptr && size > 0);

        const std::size_t offset {fields.at(field).offset};
        const std::size_t size {fields.at(field).size};

        std::memcpy(data + offset, field_data, size);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }

    void UniformBuffer::allocate_memory(std::size_t size) {
        data = new unsigned char[size];
        this->size = size;

        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
    }

    std::size_t UniformBuffer::type_size(unsigned int type) {
        std::size_t size {0};

        switch (type) {
            case GL_FLOAT_VEC3:
                size = 3 * sizeof(GLfloat);
                break;
            case GL_FLOAT:
                size = 1 * sizeof(GLfloat);
                break;
            case GL_FLOAT_MAT4:
                size = 16 * sizeof(GLfloat);
                break;
            default:
                assert(false);
                break;
        }

        return size;
    }
}
