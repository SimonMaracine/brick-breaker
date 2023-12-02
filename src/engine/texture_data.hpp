#pragma once

#include <string>

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

        const unsigned char* get_data() const { return data; }
    private:
        unsigned char* data {nullptr};
        int width {};
        int height {};

        friend class Texture;
        friend class TextureCubemap;
    };
}
