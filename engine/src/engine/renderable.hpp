#pragma once

#include <memory>
#include <functional>
#include <optional>

#include <glm/glm.hpp>

namespace bb {
    class VertexArray;
    class MaterialInstance;

    struct Renderable {
        std::weak_ptr<VertexArray> vertex_array;
        std::weak_ptr<MaterialInstance> material;

        glm::vec3 position {};
        glm::vec3 rotation {};
        float scale {1.0f};
        std::optional<glm::mat4> transformation;

        glm::vec3 outline_color {};
    };
}
