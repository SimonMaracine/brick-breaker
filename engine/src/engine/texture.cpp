#include <memory>
#include <string>
#include <array>
#include <optional>
#include <vector>
#include <cstddef>
#include <cstring>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <SDL_image.h>

#include "engine/texture_data.hpp"
#include "engine/texture.hpp"
#include "engine/panic.hpp"
#include "engine/logging.hpp"

namespace bb {
    static bool use_mipmapping(const TextureSpecification& specification) {
        return specification.mipmap_levels > 1;
    }

    static void configure_mipmapping(const TextureSpecification& specification) {
        if (!use_mipmapping(specification)) {
            return;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, specification.bias);
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

        SDL_Surface* surface {IMG_Load(file_path.c_str())};

        if (surface == nullptr) {
            log_message("Could not load texture `%s`\n", file_path.c_str());
            throw ResourceLoadingError;
        }

        width = surface->w;
        height = surface->h;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        configure_filter_and_wrap(specification);
        allocate_texture(width, height, static_cast<unsigned char*>(surface->pixels));
        configure_mipmapping(specification);

        glBindTexture(GL_TEXTURE_2D, 0);
        SDL_FreeSurface(surface);
    }

    Texture::Texture(std::shared_ptr<TextureData> data, const TextureSpecification& specification)
        : specification(specification) {
        assert(data->get_data() != nullptr);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        configure_filter_and_wrap(specification);
        allocate_texture(data->width, data->height, data->get_data());
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

    void Texture::set_pixels(int xoffset, int yoffset, int width, int height, const void* pixels) {
        switch (specification.format) {
            case Format::Rgba8:
                glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                break;
            case Format::Rgb8:
                glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
                break;
            case Format::R8:
                glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RED, GL_UNSIGNED_BYTE, pixels);
                break;
        }
    }

    void Texture::allocate_texture(int width, int height, const unsigned char* data) const {
        switch (specification.format) {
            case Format::Rgba8:
                glTexStorage2D(GL_TEXTURE_2D, specification.mipmap_levels, GL_RGBA8, width, height);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

                break;
            case Format::Rgb8:
                glTexStorage2D(GL_TEXTURE_2D, specification.mipmap_levels, GL_RGB8, width, height);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

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

        int width, height;
        SDL_Surface* data[6] {};

        for (std::size_t i {0}; i < 6; i++) {
            SDL_Surface* surface {IMG_Load(file_paths[i])};

            if (surface == nullptr) {
                log_message("Could not load texture `%s`\n", file_paths[i]);
                throw ResourceLoadingError;
            }

            width = surface->w;
            height = surface->h;

            data[i] = surface;
        }

        glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, width, height);

        for (std::size_t i {0}; i < 6; i++) {
            glTexSubImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height,
                GL_RGBA, GL_UNSIGNED_BYTE, data[i]->pixels
            );

            SDL_FreeSurface(data[i]);
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
                GL_RGBA, GL_UNSIGNED_BYTE, data[i]->get_data()
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
