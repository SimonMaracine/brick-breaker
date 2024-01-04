#include "sound_data.hpp"

#include <SDL_mixer.h>

#include "engine/panic.hpp"
#include "engine/logging.hpp"

namespace bb {
    SoundData::SoundData(const std::string& file_path) {
        Mix_Chunk* chunk {Mix_LoadWAV(file_path.c_str())};

        if (chunk == nullptr) {
            log_message("Could not load sound data `%s`\n", file_path.c_str());
            throw ResourceLoadingError;
        }

        this->chunk = chunk;
    }

    SoundData::~SoundData() {
        Mix_FreeChunk(static_cast<Mix_Chunk*>(chunk));
    }
}
