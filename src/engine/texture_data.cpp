#include <string>

#include "SDL_image.h"

#include "engine/texture_data.hpp"

namespace bb {
    TextureData::TextureData(const std::string& file_path) {
        SDL_Surface* data {IMG_Load(file_path.c_str())};
    }

    TextureData::~TextureData() {

    }
}
