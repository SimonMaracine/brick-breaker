#include <cstddef>
#include <vector>

#include <glad/glad.h>

#include "engine/vertex_buffer_layout.hpp"

namespace bb {
    std::size_t VertexBufferLayout::VertexElement::get_size(Type type) {
        switch (type) {
            case Float:
                return sizeof(GLfloat);
            case Int:
                return sizeof(GLint);
        }

        return 0;
    }

    VertexBufferLayout& VertexBufferLayout::add(unsigned int index, Type type, int size, bool per_instance) {
        VertexElement element;
        element.index = index;
        element.type = type;
        element.size = size;
        element.per_instance = per_instance;

        elements.push_back(element);
        stride += size * VertexElement::get_size(type);

        return *this;
    }
}
