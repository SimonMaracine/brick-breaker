#include <memory>
#include <vector>
#include <functional>
#include <cstddef>
#include <cassert>

#include <glad/glad.h>

#include "engine/panic.hpp"
#include "engine/vertex_array.hpp"
#include "engine/buffer.hpp"
#include "engine/vertex_buffer_layout.hpp"
#include "engine/logging.hpp"

namespace bb {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &array);
        glBindVertexArray(array);

        glBindVertexArray(0);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &array);
    }

    void VertexArray::bind() const {
        glBindVertexArray(array);
    }

    void VertexArray::unbind() {
        glBindVertexArray(0);
    }

    void VertexArray::configure(const Configuration& configuration) {
        bind();
        configuration(this);
        unbind();

        // Unbind index buffer after vertex array
        GlIndexBuffer::unbind();
    }

    void VertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer> buffer, const VertexBufferLayout& layout) {
        assert(layout.elements.size() > 0);

        buffer->bind();

        std::size_t offset {0};

        for (std::size_t i {0}; i < layout.elements.size(); i++) {
            const VertexBufferLayout::VertexElement& element {layout.elements[i]};

            switch (element.type) {
                case VertexBufferLayout::Float:
                    glVertexAttribPointer(
                        element.index,
                        element.size,
                        GL_FLOAT,
                        GL_FALSE,
                        layout.stride,
                        reinterpret_cast<void*>(offset)
                    );
                    break;
                case VertexBufferLayout::Int:
                    glVertexAttribIPointer(
                        element.index,
                        element.size,
                        GL_INT,
                        layout.stride,
                        reinterpret_cast<void*>(offset)
                    );
                    break;
            }

            glEnableVertexAttribArray(element.index);

            if (element.per_instance) {
                glVertexAttribDivisor(element.index, 1);
            }

            offset += element.size * VertexBufferLayout::VertexElement::get_size(element.type);
        }

        vertex_buffers.push_back(buffer);

        VertexBuffer::unbind();
    }

    void VertexArray::add_index_buffer(std::shared_ptr<IndexBuffer> buffer) {
        buffer->bind();

        index_buffer = buffer;
    }
}
