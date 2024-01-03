#pragma once

#include <memory>
#include <functional>
#include <optional>
#include <string>

#include <glm/glm.hpp>

namespace bb {
    class VertexArray;
    class MaterialInstance;
    class Font;

    struct Renderable {
        std::weak_ptr<VertexArray> vertex_array;
        std::weak_ptr<MaterialInstance> material;

        glm::vec3 position {};
        glm::vec3 rotation {};
        float scale {1.0f};
        std::optional<glm::mat4> transformation;

        glm::vec3 outline_color {};
    };

    struct Text {
        std::shared_ptr<Font> font;

        std::string string;
        glm::vec2 position {};
        glm::vec3 color {};
        float scale {1.0f};
        bool shadows {false};
    };
}
