#include "menu.hpp"

#include <memory>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

// https://freesound.org/people/greenvwbeetle/sounds/244656/
// https://freesound.org/people/Denis%20Chapon/sounds/109435/
// https://opengameart.org/content/start-sounds-1

void Menu::on_enter() {
    auto& data {user_data<Data>()};

    if (data.basic_font == nullptr) {
        load_font();
    }

    load_sounds();
    reload_levels();

    cam_2d.set_projection_matrix(0.0f, static_cast<float>(get_width()), 0.0f, static_cast<float>(get_height()));

    connect_event<bb::WindowResizedEvent, &Menu::on_window_resized>(this);
    connect_event<bb::KeyReleasedEvent, &Menu::on_key_released>(this);

    bb::OpenGl::clear_color(0.1f, 0.1f, 0.1f);
}

void Menu::on_exit() {

}

void Menu::on_update() {
    auto& data {user_data<Data>()};

    capture(cam_2d);

    {
        static constexpr float scale {1.8f};
        const char* string {"Brick Breaker"};
        const auto [width, height] {data.basic_font->get_string_size(string, scale)};

        bb::Text text;
        text.font = data.basic_font;
        text.string = string;
        text.position = glm::vec2(static_cast<float>(get_width() - width) / 2.0f, static_cast<float>(get_height() - height));
        text.color = glm::vec3(0.8f);
        text.scale = scale;
        text.shadows = true;
        add_text(text);
    }

    {
        static constexpr float scale {0.4f};
        const char* string1 {"Mom, I wanna play that brick breaker game! Can I have it?"};
        const char* string2 {"No! We have brick breaker at home."};
        const char* string3 {"Brick breaker at home:"};

        const auto height2 {data.basic_font->get_string_size(string2, scale).second};
        const auto height3 {data.basic_font->get_string_size(string3, scale).second};

        bb::Text text;
        text.font = data.basic_font;
        text.string = string1;
        text.position = glm::vec2(20.0f, 40.0f + height2 + height3 + 5.0f);
        text.color = glm::vec3(0.8f);
        text.scale = scale;
        text.shadows = true;
        add_text(text);

        text.string = string2;
        text.position = glm::vec2(20.0f, 40.0f + height3 + 5.0f);
        add_text(text);

        text.string = string3;
        text.position = glm::vec2(20.0f, 40.0f);
        add_text(text);
    }

    {
        int y_position {get_height() - 160};

        for (std::size_t i {0}; i < level_paths.size(); i++) {
            const auto& name {level_paths[i].second};
            const bool selected {i == static_cast<std::size_t>(level_index)};

            static constexpr float scale {0.8f};
            const auto string {selected ? "* " + name : name};

            const auto [width, height] {data.basic_font->get_string_size(string, scale)};

            bb::Text text;
            text.font = data.basic_font;
            text.string = string;
            text.position = glm::vec2(static_cast<float>(get_width() - width) / 2.0f, y_position);
            text.color = selected ? glm::vec3(0.6f, 0.9f, 0.7f) : glm::vec3(0.75f, 0.75f, 0.75f);
            text.scale = scale;
            text.shadows = true;
            add_text(text);

            y_position -= height;
        }
    }

    draw_version();
}

void Menu::on_window_resized(const bb::WindowResizedEvent& event) {
    cam_2d.set_projection_matrix(0.0f, static_cast<float>(event.width), 0.0f, static_cast<float>(event.width));
}

void Menu::on_key_released(const bb::KeyReleasedEvent& event) {
    auto& data {user_data<Data>()};

    switch (event.key) {
        case bb::KeyCode::K_UP:
            level_index = level_index - 1 < 0 ? static_cast<int>(level_paths.size() - 1u) : level_index - 1;
            play_sound(data.sound_switch);
            break;
        case bb::KeyCode::K_DOWN:
            level_index = (level_index + 1) % static_cast<int>(level_paths.size());
            play_sound(data.sound_switch);
            break;
        case bb::KeyCode::K_RETURN:
            data.selected_level = level_paths.at(level_index).first;
            change_scene("level");
            break;
        case bb::KeyCode::K_l:
            reload_levels();
            break;
        default:
            break;
    }
}

void Menu::load_font() {
    auto& data {user_data<Data>()};

    data.basic_font = std::make_shared<bb::Font>("data/fonts/CodeNewRoman/code-new-roman.regular.ttf", 40.0f, 8, 180, 40, 512);

    data.basic_font->begin_baking();
    data.basic_font->bake_ascii();
    data.basic_font->end_baking();
}

void Menu::reload_levels() {
    level_paths.clear();

    const char* levels_directory {"data/levels"};

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
}

void Menu::load_sounds() {
    auto& data {user_data<Data>()};

    data.sound_start = std::make_shared<bb::SoundData>("data/sounds/start.wav");
    data.sound_start_failure = std::make_shared<bb::SoundData>("data/sounds/start-failure.wav");
    data.sound_collision_brick = std::make_shared<bb::SoundData>("data/sounds/pop.wav");
    data.sound_collision_paddle = std::make_shared<bb::SoundData>("data/sounds/spring-bouncing.wav");
    data.sound_collision_wall = std::make_shared<bb::SoundData>("data/sounds/plastic-hit.wav");
    data.sound_die = std::make_shared<bb::SoundData>("data/sounds/die.wav");
    data.sound_lost = std::make_shared<bb::SoundData>("data/sounds/lost.wav");
    data.sound_won = std::make_shared<bb::SoundData>("data/sounds/won.wav");
    data.sound_switch = std::make_shared<bb::SoundData>("data/sounds/switch.wav");
}

std::optional<std::string> Menu::get_level_name(const std::string& file_path) {
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

void Menu::draw_version() {
    auto& data {user_data<Data>()};

    static constexpr float scale {0.3f};
    const auto string {
        "version " +
        std::to_string(VER_MAJOR) + "." +
        std::to_string(VER_MINOR) + "." +
        std::to_string(VER_PATCH)
    };

    const auto [width, _] {data.basic_font->get_string_size(string, scale)};

    bb::Text text;
    text.font = data.basic_font;
    text.string = string;
    text.position = glm::vec2(static_cast<float>(get_width() - width) - 2.0f, 2.0f);
    text.color = glm::vec3(0.9f);
    text.scale = scale;
    add_text(text);
}
