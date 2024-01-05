#pragma once

#include <memory>

#include "engine/engine.hpp"

struct Data {
    std::shared_ptr<bb::Font> basic_font;
    std::string selected_level;
    std::shared_ptr<bb::SoundData> sound_start;
    std::shared_ptr<bb::SoundData> sound_start_failure;
    std::shared_ptr<bb::SoundData> sound_collision_brick;
    std::shared_ptr<bb::SoundData> sound_collision_paddle;
    std::shared_ptr<bb::SoundData> sound_collision_wall;
    std::shared_ptr<bb::SoundData> sound_missed_ball;
    std::shared_ptr<bb::SoundData> sound_lost;
    std::shared_ptr<bb::SoundData> sound_won;
    std::shared_ptr<bb::SoundData> sound_switch;
};
