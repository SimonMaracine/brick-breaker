#include <string>

#include "SDL_image.h"

#include "engine/texture_data.hpp"
#include "engine/logging.hpp"
#include "engine/panic.hpp"

namespace bb {
    TextureData::TextureData(const std::string& file_path) {
        SDL_Surface* surface {IMG_Load(file_path.c_str())};

        if (surface == nullptr) {
            log_message("Could not load texture data `%s`\n", file_path.c_str());
            throw ResourceLoadingError;
        }

        width = surface->w;
        height = surface->h;
    }

    TextureData::~TextureData() {
        SDL_FreeSurface(surface);
    }

    const unsigned char* TextureData::get_data() const {
        return static_cast<unsigned char*>(surface->pixels);
    }
}
