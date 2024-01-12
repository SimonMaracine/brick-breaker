#include "menu.hpp"

#include <memory>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "data.hpp"

// https://freesound.org/people/greenvwbeetle/sounds/244656/
// https://freesound.org/people/Denis%20Chapon/sounds/109435/
// https://opengameart.org/content/start-sounds-1

void MenuScene::on_enter() {
    auto& data {user_data<Data>()};

    if (data.basic_font == nullptr) {
        load_font();
    }

    load_sounds();

    cam_2d.set_projection_matrix(0.0f, static_cast<float>(get_width()), 0.0f, static_cast<float>(get_height()));

    connect_event<bb::WindowResizedEvent, &MenuScene::on_window_resized>(this);
    connect_event<bb::KeyReleasedEvent, &MenuScene::on_key_released>(this);

    bb::OpenGl::clear_color(0.1f, 0.1f, 0.1f);
    capture_mouse(false);
}

void MenuScene::on_exit() {

}

void MenuScene::on_update() {
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
        static constexpr float scale {1.0f};
        const char* string1 {"Adventure Mode"};
        const char* string2 {"Custom Levels"};
        const char* string3 {"About"};
        const char* string4 {"Quit"};

        const auto string_1 {menu_selection.get() == AdventureMode ? std::string("*") + string1 : string1};
        const auto string_2 {menu_selection.get() == CustomLevels ? std::string("*") + string2 : string2};
        const auto string_3 {menu_selection.get() == About ? std::string("*") + string3 : string3};
        const auto string_4 {menu_selection.get() == Quit ? std::string("*") + string4 : string4};

        const auto [width1, height1] {data.basic_font->get_string_size(string_1, scale)};
        const auto [width2, height2] {data.basic_font->get_string_size(string_2, scale)};
        const auto [width3, height3] {data.basic_font->get_string_size(string_3, scale)};
        const auto [width4, _] {data.basic_font->get_string_size(string_4, scale)};

        bb::Text text;
        text.font = data.basic_font;
        text.string = string_1;
        text.position = glm::vec2(static_cast<float>(get_width() - width1) / 2.0f, get_height() - 170);
        text.color = glm::vec3(0.75f);
        text.scale = scale;
        text.shadows = true;
        add_text(text);

        text.string = string_2;
        text.position = glm::vec2(static_cast<float>(get_width() - width2) / 2.0f, get_height() - 170 - height1 - 10.0f);
        add_text(text);

        text.string = string_3;
        text.position = glm::vec2(static_cast<float>(get_width() - width3) / 2.0f, get_height() - 170 - height1 - height2 - 20.0f);
        add_text(text);

        text.string = string_4;
        text.position = glm::vec2(static_cast<float>(get_width() - width4) / 2.0f, get_height() - 170 - height1 - height2 - height3 - 30.0f);
        add_text(text);
    }

    draw_version();
}

void MenuScene::on_window_resized(const bb::WindowResizedEvent& event) {
    cam_2d.set_projection_matrix(0.0f, static_cast<float>(event.width), 0.0f, static_cast<float>(event.height));
}

void MenuScene::on_key_released(const bb::KeyReleasedEvent& event) {
    auto& data {user_data<Data>()};

    switch (event.key) {
        case bb::KeyCode::K_UP:
            menu_selection.up();
            play_sound(data.sound_switch);
            break;
        case bb::KeyCode::K_DOWN:
            menu_selection.down();
            play_sound(data.sound_switch);
            break;
        case bb::KeyCode::K_RETURN:
            execute_selection(menu_selection.get());
            break;
        default:
            break;
    }
}

void MenuScene::load_font() {
    auto& data {user_data<Data>()};

    data.basic_font = std::make_shared<bb::Font>("data/fonts/CodeNewRoman/code-new-roman.regular.ttf", 40.0f, 8, 180, 40, 512);

    data.basic_font->begin_baking();
    data.basic_font->bake_ascii();
    data.basic_font->bake_characters(u8"ă");
    data.basic_font->end_baking();
}

void MenuScene::load_sounds() {
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

void MenuScene::execute_selection(int selection) {
    auto& data {user_data<Data>()};

    switch (selection) {
        case AdventureMode:
            data.selected_level = ADVENTURE_LEVELS[data.current_level];
            change_scene("level");
            break;
        case CustomLevels:
            change_scene("levels");
            break;
        case About:
            change_scene("about");
            break;
        case Quit:
            quit_application();
            break;
    }
}

void MenuScene::draw_version() {
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
