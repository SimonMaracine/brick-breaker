#include "audio.hpp"

#include <SDL_mixer.h>

#include "engine/panic.hpp"
#include "engine/logging.hpp"

namespace bb {
    AudioManager::AudioManager() {
        Mix_Init(0);

        if (Mix_OpenAudio(44100, AUDIO_S16, 1, 4096) < 0) {
            log_message("Could not open audio device\n");
            throw InitializationError;
        }

        if (Mix_AllocateChannels(8) != 8) {
            log_message("Could not allocate all channels\n");
        }
    }

    AudioManager::~AudioManager() {
        Mix_CloseAudio();
        Mix_Quit();
    }

    void AudioManager::play_sound(std::shared_ptr<SoundData> sound_data) {
        Mix_PlayChannel(-1, static_cast<Mix_Chunk*>(sound_data->get_chunk()), 0);
    }

    void AudioManager::pause_sound() {
        Mix_Pause(-1);
    }

    void AudioManager::resume_sound() {
        Mix_Resume(-1);
    }
}
