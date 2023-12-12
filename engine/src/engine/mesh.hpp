#pragma once

#include <cstddef>
#include <string>

namespace bb {
    class Mesh {
    public:
        enum class Type {
            P,
            PN,
            PTN,
            PTNT
        };

        static constexpr const char* DEFAULT_OBJECT {"defaultobject"};

        Mesh(const std::string& file_path, const std::string& object_name, Type type, bool flip_winding = false);
        ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) = delete;
        Mesh& operator=(Mesh&&) = delete;

        const void* get_vertices() const { return vertices; }
        const void* get_indices() const { return indices; }
        std::size_t get_vertices_size() const { return vertices_size; }
        std::size_t get_indices_size() const { return indices_size; }
    private:
        void load(Type type, const void* pmesh);
        void allocate(const void* vertices, std::size_t vertices_size, const void* indices, std::size_t indices_size);

        unsigned char* vertices {nullptr};
        unsigned char* indices {nullptr};

        std::size_t vertices_size {0};
        std::size_t indices_size {0};
    };
}
