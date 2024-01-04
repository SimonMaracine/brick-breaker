#pragma once

#include <memory>

#include "engine/engine.hpp"

struct Data {
    std::shared_ptr<bb::Font> basic_font;
    std::string selected_level;
};
