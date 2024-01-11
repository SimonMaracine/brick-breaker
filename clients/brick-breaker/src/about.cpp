#include "about.hpp"

#include "data.hpp"

void AboutScene::on_enter() {
    cam_2d.set_projection_matrix(0.0f, static_cast<float>(get_width()), 0.0f, static_cast<float>(get_height()));

    connect_event<bb::WindowResizedEvent, &AboutScene::on_window_resized>(this);
    connect_event<bb::KeyReleasedEvent, &AboutScene::on_key_released>(this);
}

void AboutScene::on_exit() {

}

void AboutScene::on_update() {
    auto& data {user_data<Data>()};

    capture(cam_2d);

    static constexpr float scale {1.2f};
    const char* string {u8"Made by Simon Mărăcine :)"};
    const auto [width, height] {data.basic_font->get_string_size(string, scale)};

    bb::Text text;
    text.font = data.basic_font;
    text.string = string;
    text.position = glm::vec2(static_cast<float>(get_width() - width) / 2.0f, static_cast<float>(get_height() - height) / 2.0f);
    text.color = glm::vec3(0.8f);
    text.scale = scale;
    text.shadows = true;
    add_text(text);
}

void AboutScene::on_window_resized(const bb::WindowResizedEvent& event) {
    cam_2d.set_projection_matrix(0.0f, static_cast<float>(event.width), 0.0f, static_cast<float>(event.height));
}

void AboutScene::on_key_released(const bb::KeyReleasedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_ESCAPE:
            change_scene("menu");
            break;
        default:
            break;
    }
}
