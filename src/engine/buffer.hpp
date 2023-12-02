#pragma once

#include <cstddef>

namespace bb {
    enum class DrawHint {
        Static,
        Dynamic,
        Stream
    };

    class VertexBuffer {
    public:
        VertexBuffer(DrawHint hint = DrawHint::Static);
        VertexBuffer(std::size_t size, DrawHint hint = DrawHint::Static);
        VertexBuffer(const void* data, std::size_t size, DrawHint hint = DrawHint::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;

        void bind() const;
        static void unbind();

        void upload_data(const void* data, std::size_t size) const;
        void upload_sub_data(const void* data, std::size_t offset, std::size_t size) const;
    private:
        unsigned int buffer {0};
        DrawHint hint {DrawHint::Static};
    };

    // Only supports unsigned int
    class IndexBuffer {
    public:
        IndexBuffer(const void* data, std::size_t size);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) = delete;
        IndexBuffer& operator=(IndexBuffer&&) = delete;

        void bind() const;
        static void unbind();

        int get_index_count() const { return index_count; }
    private:
        unsigned int buffer {0};
        int index_count {0};
    };
}
