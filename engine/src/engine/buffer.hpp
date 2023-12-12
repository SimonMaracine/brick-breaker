#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <resmanager/resmanager.hpp>

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

    struct UniformBlockSpecification {
        std::string block_name;
        std::vector<std::string> uniforms;
        unsigned int binding_index {};
    };

    class UniformBuffer {
    public:
        using Key = resmanager::HashedStr64;
        using KeyHash = resmanager::Hash<Key>;

        UniformBuffer(const UniformBlockSpecification& specification);
        ~UniformBuffer();

        UniformBuffer(const UniformBuffer&) = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;
        UniformBuffer(UniformBuffer&&) = delete;
        UniformBuffer& operator=(UniformBuffer&&) = delete;

        void bind() const;
        static void unbind();

        bool is_configured() const { return configured; }
        void configure(unsigned int shader_program);

        void set(const void* field_data, Key field);
        void upload() const;
        void set_and_upload(const void* field_data, Key field);
    private:
        void allocate_memory(std::size_t size);
        static std::size_t type_size(unsigned int type);

        unsigned int buffer {0};

        struct UniformBlockField {
            std::size_t offset {0};
            std::size_t size {0};
        };

        UniformBlockSpecification specification;

        unsigned char* data {nullptr};
        std::size_t size {0};

        std::unordered_map<Key, UniformBlockField, KeyHash> fields;

        bool configured {false};

        friend class Shader;
    };
}
