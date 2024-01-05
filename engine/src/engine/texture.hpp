#pragma once

#include <memory>
#include <string>
#include <array>
#include <optional>

#include <glm/glm.hpp>

#include "engine/texture_data.hpp"

namespace bb {
    enum class Filter {
        Linear,
        Nearest
    };

    enum class Wrap {
        Repeat,
        ClampEdge,
        ClampBorder
    };

    enum class Format {
        Rgba8,
        Rgb8,
        R8
    };

    struct TextureSpecification {
        Format format {Format::Rgba8};

        Filter min_filter {Filter::Linear};
        Filter mag_filter {Filter::Linear};

        Wrap wrap_s {Wrap::ClampBorder};
        Wrap wrap_t {Wrap::ClampBorder};

        std::optional<glm::vec4> border_color;

        // Mipmapping is off by default
        int mipmap_levels {1};
        float bias {0.0f};
    };

    class Texture {
    public:
        Texture(const std::string& file_path, const TextureSpecification& specification);
        Texture(std::shared_ptr<TextureData> data, const TextureSpecification& specification);
        Texture(int width, int height, unsigned char* data, const TextureSpecification& specification);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&&) = delete;
        Texture& operator=(Texture&&) = delete;

        int get_width() const { return width; }
        int get_height() const { return height; }
        unsigned int get_id() const { return texture; }

        void bind(unsigned int unit) const;
        static void unbind();
    private:
        void allocate_texture(int width, int height, const unsigned char* data) const;

        TextureSpecification specification;

        unsigned int texture {0};
        int width {0};
        int height {0};
    };

    class TextureCubemap {
    public:
        // Textures need to be RGBA
        TextureCubemap(const char** file_paths);
        TextureCubemap(const std::array<std::shared_ptr<TextureData>, 6>& data);
        ~TextureCubemap();

        TextureCubemap(const TextureCubemap&) = delete;
        TextureCubemap& operator=(const TextureCubemap&) = delete;
        TextureCubemap(TextureCubemap&&) = delete;
        TextureCubemap& operator=(TextureCubemap&&) = delete;

        void bind(unsigned int unit) const;
        static void unbind();
    private:
        unsigned int texture {0};
    };

    inline constexpr float CUBEMAP_VERTICES[] {
        -5.0f,  5.0f, -5.0f,
        -5.0f, -5.0f, -5.0f,
         5.0f, -5.0f, -5.0f,
         5.0f, -5.0f, -5.0f,
         5.0f,  5.0f, -5.0f,
        -5.0f,  5.0f, -5.0f,

        -5.0f, -5.0f,  5.0f,
        -5.0f, -5.0f, -5.0f,
        -5.0f,  5.0f, -5.0f,
        -5.0f,  5.0f, -5.0f,
        -5.0f,  5.0f,  5.0f,
        -5.0f, -5.0f,  5.0f,

         5.0f, -5.0f, -5.0f,
         5.0f, -5.0f,  5.0f,
         5.0f,  5.0f,  5.0f,
         5.0f,  5.0f,  5.0f,
         5.0f,  5.0f, -5.0f,
         5.0f, -5.0f, -5.0f,

        -5.0f, -5.0f,  5.0f,
        -5.0f,  5.0f,  5.0f,
         5.0f,  5.0f,  5.0f,
         5.0f,  5.0f,  5.0f,
         5.0f, -5.0f,  5.0f,
        -5.0f, -5.0f,  5.0f,

        -5.0f,  5.0f, -5.0f,
         5.0f,  5.0f, -5.0f,
         5.0f,  5.0f,  5.0f,
         5.0f,  5.0f,  5.0f,
        -5.0f,  5.0f,  5.0f,
        -5.0f,  5.0f, -5.0f,

        -5.0f, -5.0f, -5.0f,
        -5.0f, -5.0f,  5.0f,
         5.0f, -5.0f, -5.0f,
         5.0f, -5.0f, -5.0f,
        -5.0f, -5.0f,  5.0f,
         5.0f, -5.0f,  5.0f
    };
}
