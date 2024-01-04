#pragma once

#include <memory>

#include "engine/sound_data.hpp"

namespace bb {
    class AudioManager {
    public:
        AudioManager();
        ~AudioManager();

        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;
        AudioManager(AudioManager&&) = delete;
        AudioManager& operator=(AudioManager&&) = delete;

        void play_sound(std::shared_ptr<SoundData> sound_data);
        void pause_sound();
        void resume_sound();
    };
}
