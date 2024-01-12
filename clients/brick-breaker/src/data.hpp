#pragma once

#include <memory>
#include <cstddef>

#include "engine/engine.hpp"

inline const char* ADVENTURE_LEVELS[] {
    "data/levels/adventure/level1.json",
    "data/levels/adventure/level2.json",
    "data/levels/adventure/level3.json",
    "data/levels/adventure/level4.json"
};

inline constexpr std::size_t ADVENTURE_MAX {sizeof(ADVENTURE_LEVELS) / sizeof(ADVENTURE_LEVELS[0])};

struct Data {
    std::shared_ptr<bb::Font> basic_font;

    std::string selected_level;
    std::size_t current_level {0u};

    std::shared_ptr<bb::SoundData> sound_start;
    std::shared_ptr<bb::SoundData> sound_start_failure;
    std::shared_ptr<bb::SoundData> sound_collision_brick;
    std::shared_ptr<bb::SoundData> sound_collision_paddle;
    std::shared_ptr<bb::SoundData> sound_collision_wall;
    std::shared_ptr<bb::SoundData> sound_die;
    std::shared_ptr<bb::SoundData> sound_lost;
    std::shared_ptr<bb::SoundData> sound_won;
    std::shared_ptr<bb::SoundData> sound_switch;

    unsigned int lives {3u};
    int score {0};

    bool mouse_input {false};
};
