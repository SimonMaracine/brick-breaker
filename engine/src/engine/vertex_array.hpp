#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "engine/vertex_buffer_layout.hpp"

namespace bb {
    class VertexBuffer;
    class IndexBuffer;

    class VertexArray {
    public:
        using Configuration = std::function<void(VertexArray*)>;

        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&&) = delete;
        VertexArray& operator=(VertexArray&&) = delete;

        void bind() const;
        static void unbind();

        void configure(const Configuration& configuration);

        void add_vertex_buffer(std::shared_ptr<VertexBuffer> buffer, const VertexBufferLayout& layout);
        void add_index_buffer(std::shared_ptr<IndexBuffer> buffer);

        const IndexBuffer* get_index_buffer() const { return index_buffer.get(); }
    private:
        unsigned int array {0};

        std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;
        std::shared_ptr<IndexBuffer> index_buffer;
    };
}
