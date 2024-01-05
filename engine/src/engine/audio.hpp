#pragma once

#include <memory>

#include "engine/sound_data.hpp"

namespace bb {
    struct AudioManager {
        static void initialize();
        static void uninitialize();

        static void play_sound(std::shared_ptr<SoundData> sound_data);
        static void pause_sound();
        static void resume_sound();
    };
}
