#pragma once

#include <string>

struct SDL_Surface;

namespace bb {
    class Texture;
    class TextureCubemap;

    class TextureData {
    public:
        TextureData(const std::string& file_path);
        ~TextureData();

        TextureData(const TextureData&) = delete;
        TextureData& operator=(const TextureData&) = delete;
        TextureData(TextureData&&) = delete;
        TextureData& operator=(TextureData&&) = delete;

        const unsigned char* get_data() const;
    private:
        SDL_Surface* surface {nullptr};
        int width {};
        int height {};

        friend class Texture;
        friend class TextureCubemap;
    };
}
