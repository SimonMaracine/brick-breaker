#include <memory>
#include <string>
#include <array>
#include <optional>
#include <vector>
#include <cstddef>
#include <cstring>
#include <cassert>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/texture.hpp"
#include "engine/panic.hpp"
#include "engine/logging.hpp"
// #include "engine/application_base/capabilities.hpp"
// #include "engine/graphics/texture_data.hpp"

namespace bb {
    static constexpr int CHANNELS {4};

    static bool use_mipmapping(const TextureSpecification& specification) {
        return specification.mipmap_levels > 1;
    }

    static void configure_mipmapping(const TextureSpecification& specification) {
        if (!use_mipmapping(specification)) {
            return;
        }

        const bool anisotropic_filtering_enabled {specification.anisotropic_filtering > 0};

        const float bias {anisotropic_filtering_enabled ? 0.0f : specification.bias};

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, bias);

        if (anisotropic_filtering_enabled) {
            assert(specification.anisotropic_filtering <= max_anisotropic_filtering_supported());

            const float amount {static_cast<float>(specification.anisotropic_filtering)};

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
        }
    }

    static int filter_to_int(Filter filter) {
        int result {0};

        switch (filter) {
            case Filter::Linear:
                result = GL_LINEAR;
                break;
            case Filter::Nearest:
                result = GL_NEAREST;
                break;
        }

        return result;
    }

    static int wrap_to_int(Wrap wrap) {
        int result {0};

        switch (wrap) {
            case Wrap::Repeat:
                result = GL_REPEAT;
                break;
            case Wrap::ClampEdge:
                result = GL_CLAMP_TO_EDGE;
                break;
            case Wrap::ClampBorder:
                result = GL_CLAMP_TO_BORDER;
                break;
        }

        return result;
    }

    static void configure_filter_and_wrap(const TextureSpecification& specification) {
        const int min_filter {
            use_mipmapping(specification) ? GL_LINEAR_MIPMAP_LINEAR : filter_to_int(specification.min_filter)
        };

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_to_int(specification.mag_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_to_int(specification.wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_to_int(specification.wrap_t));

        if (specification.border_color != std::nullopt) {
            const glm::vec4& color {specification.border_color.value()};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
        }
    }

    static void configure_filter_and_wrap_3d() {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    Texture::Texture(const std::string& file_path, const TextureSpecification& specification)
        : specification(specification) {
        stbi_set_flip_vertically_on_load(1);

        int width, height, channels;
        unsigned char* data {stbi_load(file_path.c_str(), &width, &height, &channels, CHANNELS)};

        if (data == nullptr) {
            log_message("Could not load texture `%s`", file_path.c_str());
            throw ResourceLoadingError;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        configure_filter_and_wrap(specification);
        allocate_texture(width, height, data);
        configure_mipmapping(specification);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        this->width = width;
        this->height = height;
    }

    Texture::Texture(std::shared_ptr<TextureData> data, const TextureSpecification& specification)
        : specification(specification) {
        (data->data != nullptr);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        configure_filter_and_wrap(specification);
        allocate_texture(data->width, data->height, data->data);
        configure_mipmapping(specification);

        glBindTexture(GL_TEXTURE_2D, 0);

        width = data->width;
        height = data->height;
    }

    Texture::Texture(int width, int height, unsigned char* data, const TextureSpecification& specification)
        : specification(specification) {
        assert(data != nullptr);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        configure_filter_and_wrap(specification);
        allocate_texture(width, height, data);
        configure_mipmapping(specification);

        glBindTexture(GL_TEXTURE_2D, 0);

        this->width = width;
        this->height = height;
    }

    Texture::~Texture() {
        glDeleteTextures(1, &texture);
    }

    void Texture::bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::allocate_texture(int width, int height, unsigned char* data) const {
        switch (specification.format) {
            case Format::Rgba8:
                glTexStorage2D(GL_TEXTURE_2D, specification.mipmap_levels, GL_RGBA8, width, height);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

                break;
            case Format::R8:
                glTexStorage2D(GL_TEXTURE_2D, specification.mipmap_levels, GL_R8, width, height);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, data);

                break;
        }
    }

    TextureCubemap::TextureCubemap(const char** file_paths) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        configure_filter_and_wrap_3d();

        stbi_set_flip_vertically_on_load(0);

        int width, height, channels;
        unsigned char* data[6];

        for (std::size_t i {0}; i < 6; i++) {
            data[i] = stbi_load(file_paths[i], &width, &height, &channels, CHANNELS);

            if (data[i] == nullptr) {
                log_message("Could not load texture `%s`", file_paths[i]);
                throw ResourceLoadingError;
            }
        }

        glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, width, height);

        for (std::size_t i {0}; i < 6; i++) {
            glTexSubImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height,
                GL_RGBA, GL_UNSIGNED_BYTE, data[i]
            );

            stbi_image_free(data[i]);
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    TextureCubemap::TextureCubemap(const std::array<std::shared_ptr<TextureData>, 6>& data) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        configure_filter_and_wrap_3d();

        glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, data[0]->width, data[0]->height);

        for (std::size_t i {0}; i < 6; i++) {
            glTexSubImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, data[i]->width, data[i]->height,
                GL_RGBA, GL_UNSIGNED_BYTE, data[i]->data
            );
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    TextureCubemap::~TextureCubemap() {
        glDeleteTextures(1, &texture);
    }

    void TextureCubemap::bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    }

    void TextureCubemap::unbind() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}
