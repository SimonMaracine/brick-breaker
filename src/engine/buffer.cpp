#include <cstddef>
#include <cassert>

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
}
