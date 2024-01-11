#include "levels.hpp"

#include <memory>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "data.hpp"

void LevelsScene::on_enter() {
    const auto levels {reload_levels()};
    level_selection = MenuSelection(static_cast<int>(levels));

    cam_2d.set_projection_matrix(0.0f, static_cast<float>(get_width()), 0.0f, static_cast<float>(get_height()));

    connect_event<bb::WindowResizedEvent, &LevelsScene::on_window_resized>(this);
    connect_event<bb::KeyReleasedEvent, &LevelsScene::on_key_released>(this);
}

void LevelsScene::on_exit() {

}

void LevelsScene::on_update() {
    auto& data {user_data<Data>()};

    capture(cam_2d);

    {
        int y_position {get_height() - 120};

        for (std::size_t i {0}; i < level_paths.size(); i++) {
            const auto& name {level_paths[i].second};
            const bool selected {i == static_cast<std::size_t>(level_selection.get())};

            static constexpr float scale {1.2f};
            const auto string {selected ? "* " + name : name};

            const auto [width, height] {data.basic_font->get_string_size(string, scale)};

            bb::Text text;
            text.font = data.basic_font;
            text.string = string;
            text.position = glm::vec2(static_cast<float>(get_width() - width) / 2.0f, y_position);
            text.color = glm::vec3(0.75f);
            text.scale = scale;
            text.shadows = true;
            add_text(text);

            y_position -= height + 6.0f;
        }
    }
}

void LevelsScene::on_window_resized(const bb::WindowResizedEvent& event) {
    cam_2d.set_projection_matrix(0.0f, static_cast<float>(event.width), 0.0f, static_cast<float>(event.height));
}

void LevelsScene::on_key_released(const bb::KeyReleasedEvent& event) {
    auto& data {user_data<Data>()};

    switch (event.key) {
        case bb::KeyCode::K_UP:
            level_selection.up();
            play_sound(data.sound_switch);
            break;
        case bb::KeyCode::K_DOWN:
            level_selection.down();
            play_sound(data.sound_switch);
            break;
        case bb::KeyCode::K_RETURN:
            data.selected_level = level_paths.at(static_cast<std::size_t>(level_selection.get())).first;
            change_scene("level");
            break;
        case bb::KeyCode::K_l: {
            const auto levels {reload_levels()};
            level_selection = MenuSelection(static_cast<int>(levels));
            break;
        }
        case bb::KeyCode::K_ESCAPE:
            change_scene("menu");
            break;
        default:
            break;
    }
}

std::size_t LevelsScene::reload_levels() {
    level_paths.clear();

    const char* levels_directory {"data/levels/custom"};

    const std::filesystem::path levels_path {levels_directory};

    for (const auto& entry : std::filesystem::directory_iterator(levels_path)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        const auto file {entry.path()};

        if (file.extension().string() != ".json") {
            continue;
        }

        const auto name {get_level_name(file.string())};

        if (!name) {
            continue;
        }

        level_paths.push_back(std::make_pair(file.string(), *name));

        if (level_paths.size() == static_cast<std::size_t>(MAX_LEVELS)) {
            break;
        }
    }

    return level_paths.size();
}

std::optional<std::string> LevelsScene::get_level_name(const std::string& file_path) {
    std::ifstream file {file_path};

    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string result;

    const nlohmann::json root = nlohmann::json::parse(file);

    try {
        const auto name {root["name"].get<std::string>()};

        result = std::move(name);
    } catch (const nlohmann::json::exception&) {
        return std::nullopt;
    }

    return std::make_optional(result);
}
