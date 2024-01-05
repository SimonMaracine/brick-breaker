#include "level.hpp"

#include <fstream>
#include <utility>
#include <cassert>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <nlohmann/json.hpp>

#include "constants.hpp"
#include "data.hpp"

#ifdef BB_RELEASE
    #define SHOW_DEBUG_RENDERING 0
#else
    #define SHOW_DEBUG_RENDERING 1
#endif

// https://www.goodtextures.com/image/19536/wood-bare-0256
// https://stackoverflow.com/questions/39280104/how-to-get-current-camera-position-from-view-matrix

using namespace resmanager::literals;

static constexpr float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void LevelScene::on_enter() {
    cam_controller = MyCameraController(
        &cam,
        get_width(),
        get_height(),
        LENS_FOV,
        LENS_NEAR,
        LENS_FAR,
        glm::vec3(0.0f),
        24.0f,
        32.0f
    );

    cam_controller.angle_around_point += 180.0f;

    connect_event<bb::KeyPressedEvent, &MyCameraController::on_key_pressed>(cam_controller);

    cam_2d.set_projection_matrix(0.0f, static_cast<float>(get_width()), 0.0f, static_cast<float>(get_height()));

    directional_light.direction = glm::normalize(glm::vec3(-0.6f, -1.0f, -0.5f));
    directional_light.ambient_color = glm::vec3(0.02f);
    directional_light.diffuse_color = glm::vec3(0.8f);
    directional_light.specular_color = glm::vec3(1.0f);

    lamp_left.position = LAMP_LIGHT_LEFT_POSITION;
    lamp_left.ambient_color = glm::vec3(0.03f);
    lamp_left.diffuse_color = glm::vec3(0.9f);
    lamp_left.specular_color = glm::vec3(1.0f);

    lamp_right.position = LAMP_LIGHT_RIGHT_POSITION;
    lamp_right.ambient_color = glm::vec3(0.03f);
    lamp_right.diffuse_color = glm::vec3(0.9f);
    lamp_right.specular_color = glm::vec3(1.0f);

    connect_event<bb::WindowResizedEvent, &LevelScene::on_window_resized>(this);
    connect_event<bb::KeyPressedEvent, &LevelScene::on_key_pressed>(this);
    connect_event<bb::KeyReleasedEvent, &LevelScene::on_key_released>(this);

    connect_event<BallPaddleCollisionEvent, &LevelScene::on_ball_paddle_collision>(this);
    connect_event<BallMissEvent, &LevelScene::on_ball_miss>(this);
    connect_event<OrbMissEvent, &LevelScene::on_orb_miss>(this);
    connect_event<OrbPaddleCollisionEvent, &LevelScene::on_orb_paddle_collision>(this);
    connect_event<BallBrickCollisionEvent, &LevelScene::on_ball_brick_collision>(this);

    bb::OpenGl::clear_color(0.1f, 0.1f, 0.15f);

    load_shaders();
    load_platform();
    load_ball();
    load_paddle();
    load_brick();
    load_lamp();
    load_orb();

    id_gen = IdGenerator();
    paddle = Paddle();

    balls.clear();
    create_ball();

    auto& data {user_data<Data>()};

    bricks.clear();
    const auto level {load_level(data.selected_level, id_gen)};

    if (!level) {
        bb::log_message("Could not load level!\n");
        play_sound(data.sound_start_failure);
    } else {
        bricks = std::move(*level);
        play_sound(data.sound_start);
    }

    lives = 3u;
    score = 0;
    game_over = GameOver::None;
}

void LevelScene::on_exit() {

}

void LevelScene::on_update() {
    cam_controller.update_controls(get_delta());
    cam_controller.update_camera(get_delta());

    capture(cam, cam_controller.get_position());
    capture(cam_2d);
    add_light(directional_light);
    add_light(lamp_left);
    add_light(lamp_right);
    shadows(-21.0f, 20.0f, -12.0f, 12.0f, 1.0f, 20.0f, directional_light.direction * -10.0f);

    update_bricks();

    update_paddle(paddle);

    for (auto& [_, ball] : balls) {
        update_ball(ball);
    }

    for (auto& [_, orb] : orbs) {
        update_orb(orb);
    }

    update_collisions();

    {
        bb::Renderable r_platform;
        r_platform.vertex_array = cache_vertex_array["platform"_H];
        r_platform.material = cache_material_instance["platform"_H];
        r_platform.rotation.y = glm::radians(90.0f);
        add_renderable(r_platform);
    }

    if (game_over == GameOver::None) {
        bb::Renderable r_paddle;
        r_paddle.vertex_array = cache_vertex_array["paddle"_H];
        r_paddle.material = cache_material_instance["paddle"_H];
        r_paddle.position = paddle.get_position();
        r_paddle.rotation = paddle.get_rotation();
        r_paddle.scale = paddle.get_scale();
        add_renderable(r_paddle);

#if SHOW_DEBUG_RENDERING
        Box b;
        b.position = paddle.get_position();
        b.width = paddle.get_dimensions().x;
        b.height = paddle.get_dimensions().y;
        b.depth = paddle.get_dimensions().z;
        draw_bounding_box(b);
#endif

        for (const auto& [_, ball] : balls) {
            bb::Renderable r_ball;
            r_ball.vertex_array = cache_vertex_array["ball"_H];
            r_ball.material = cache_material_instance["ball"_H];
            r_ball.transformation = ball.transformation;
            add_renderable(r_ball);

#if SHOW_DEBUG_RENDERING
            debug_add_line(ball.get_position() - glm::vec3(ball.radius, 0.0f, 0.0f), ball.get_position() + glm::vec3(ball.radius, 0.0f, 0.0f), GREEN);
            debug_add_line(ball.get_position() - glm::vec3(0.0f, 0.0f, ball.radius), ball.get_position() + glm::vec3(0.0f, 0.0f, ball.radius), GREEN);
#endif
        }

        for (const auto& [_, orb] : orbs) {
            const auto material_id {resmanager::HashedStr64("orb" + std::to_string(static_cast<int>(orb.get_type())))};

            bb::Renderable r_orb;
            r_orb.vertex_array = cache_vertex_array["orb"_H];
            r_orb.material = cache_material_instance[material_id];
            r_orb.position = orb.position;
            r_orb.scale = orb.radius;
            add_renderable(r_orb);

#if SHOW_DEBUG_RENDERING
            debug_add_line(orb.position - glm::vec3(orb.radius, 0.0f, 0.0f), orb.position + glm::vec3(orb.radius, 0.0f, 0.0f), GREEN);
            debug_add_line(orb.position - glm::vec3(0.0f, 0.0f, orb.radius), orb.position + glm::vec3(0.0f, 0.0f, orb.radius), GREEN);
#endif
        }
    }

    for (const auto& [_, brick] : bricks) {
        const auto material_id {resmanager::HashedStr64("brick" + std::to_string(static_cast<int>(brick.get_type()) + 1))};

        bb::Renderable r_brick;
        r_brick.vertex_array = cache_vertex_array["brick"_H];
        r_brick.material = cache_material_instance[material_id];
        r_brick.position = brick.get_position();
        r_brick.scale = brick.get_scale();
        add_renderable(r_brick);

#if SHOW_DEBUG_RENDERING
        Box b;
        b.position = brick.get_position();
        b.width = brick.get_dimensions().x;
        b.height = brick.get_dimensions().y;
        b.depth = brick.get_dimensions().z;
        draw_bounding_box(b);
#endif
    }

    {
        bb::Renderable r_lamp;
        r_lamp.vertex_array = cache_vertex_array["lamp_stand"_H];
        r_lamp.material = cache_material_instance["lamp_stand"_H];
        r_lamp.scale = 0.37f;
        r_lamp.position = LAMP_LEFT_POSITION;
        add_renderable(r_lamp);

        r_lamp.position = LAMP_RIGHT_POSITION;
        add_renderable(r_lamp);

        r_lamp.vertex_array = cache_vertex_array["lamp_bulb"_H];
        r_lamp.material = cache_material_instance["lamp_bulb"_H];
        r_lamp.position = LAMP_LEFT_POSITION;
        add_renderable(r_lamp);

        r_lamp.position = LAMP_RIGHT_POSITION;
        add_renderable(r_lamp);
    }

    auto& data {user_data<Data>()};

    {
        static constexpr float scale {0.8f};
        const auto string {"Score: " + std::to_string(score)};
        const auto [_, height] {data.basic_font->get_string_size(string, scale)};

        bb::Text text;
        text.font = data.basic_font;
        text.string = string;
        text.position = glm::vec2(2.0f, static_cast<float>(get_height() - height));
        text.color = glm::vec3(0.4f, 0.5f, 0.9f);
        text.scale = scale;
        text.shadows = true;
        add_text(text);
    }

    {
        std::string string;
        string.append(lives, '*');

        static constexpr float scale {1.3f};
        const auto [width, height] {data.basic_font->get_string_size(string, scale)};

        bb::Text text;
        text.font = data.basic_font;
        text.string = string;
        text.position = glm::vec2(static_cast<float>(get_width() - width), static_cast<float>(get_height() - height));
        text.color = glm::vec3(0.9f, 0.5f, 0.4f);
        text.scale = scale;
        text.shadows = true;
        add_text(text);
    }

    if (game_over != GameOver::None) {
        const char* string {nullptr};

        switch (game_over) {
            case GameOver::Lost:
                string = "Game Over";
                break;
            case GameOver::Won:
                string = "Congratulations";
                break;
            default:
                assert(false);
                break;
        }

        static constexpr float scale {2.0f};
        const auto [width, height] {data.basic_font->get_string_size(string, scale)};

        bb::Text text;
        text.font = data.basic_font;
        text.string = string;
        text.position = glm::vec2(static_cast<float>(get_width() - width), static_cast<float>(get_height() - height)) / 2.0f;
        text.color = glm::vec3(0.8f);
        text.scale = scale;
        text.shadows = true;
        add_text(text);
    }

#if SHOW_DEBUG_RENDERING
    debug_add_lamp(LAMP_LIGHT_LEFT_POSITION, glm::vec3(0.7f, 0.7f, 0.1f));
    debug_add_lamp(LAMP_LIGHT_RIGHT_POSITION, glm::vec3(0.7f, 0.7f, 0.1f));

    debug_add_line(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    debug_add_line(glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, DEADLINE_Z), glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, DEADLINE_Z), GREEN);
    debug_add_line(glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, PLATFORM_EDGE_MIN_Z), glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, PLATFORM_EDGE_MIN_Z), GREEN);
    debug_add_line(glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, PLATFORM_EDGE_MIN_Z), glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, DEADLINE_Z), GREEN);
    debug_add_line(glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, PLATFORM_EDGE_MIN_Z), glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, DEADLINE_Z), GREEN);

    draw_fps();
#endif
}

void LevelScene::on_window_resized(const bb::WindowResizedEvent& event) {
    cam.set_projection_matrix(event.width, event.height, LENS_FOV, LENS_NEAR, LENS_FAR);
    cam_2d.set_projection_matrix(0.0f, static_cast<float>(get_width()), 0.0f, static_cast<float>(get_height()));
}

void LevelScene::on_key_pressed(const bb::KeyPressedEvent& event) {
    if (!event.repeat) {
        switch (event.key) {
            case bb::KeyCode::K_LEFT:
                paddle.velocity_x = -PADDLE_VELOCITY;
                break;
            case bb::KeyCode::K_RIGHT:
                paddle.velocity_x = PADDLE_VELOCITY;
                break;
            default:
                break;
        }
    }
}

void LevelScene::on_key_released(const bb::KeyReleasedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_LEFT:
        case bb::KeyCode::K_RIGHT:
            paddle.velocity_x = 0.0f;
            break;
        case bb::KeyCode::K_SPACE:
            shoot_balls();
            break;
        case bb::KeyCode::K_r:
            change_scene("level");
            break;
        case bb::KeyCode::K_ESCAPE:
            change_scene("menu");
            break;
        default:
            break;
    }
}

void LevelScene::load_shaders() {
#if 0
    {
        // This is a generic shader
        auto shader {std::make_shared<bb::Shader>(
            "data/shaders/simple.vert",
            "data/shaders/simple.frag"
        )};

        add_shader(shader);

        // And a generic material
        auto material {cache_material.load("simple"_H, shader)};
        material->add_uniform(bb::Material::Uniform::Vec3, "u_material.ambient_diffuse"_H);
        material->add_uniform(bb::Material::Uniform::Vec3, "u_material.specular"_H);
        material->add_uniform(bb::Material::Uniform::Float, "u_material.shininess"_H);
    }

    {
        // This is also a generic shader
        auto shader {std::make_shared<bb::Shader>(
            "data/shaders/simple_textured.vert",
            "data/shaders/simple_textured.frag",
            "data/shaders/common"
        )};

        add_shader(shader);

        // And a generic material
        auto material {cache_material.load("simple_textured"_H, shader)};
        material->add_texture("u_material.ambient_diffuse"_H);
        material->add_uniform(bb::Material::Uniform::Vec3, "u_material.specular"_H);
        material->add_uniform(bb::Material::Uniform::Float, "u_material.shininess"_H);
    }
#endif

    {
        auto shader {std::make_shared<bb::Shader>(
            "data/shaders/flat.vert",
            "data/shaders/flat.frag"
        )};

        add_shader(shader);

        auto material {cache_material.load("flat"_H, shader)};
        material->add_uniform(bb::Material::Uniform::Vec3, "u_material.color"_H);
    }

    {
        auto shader {std::make_shared<bb::Shader>(
            "data/shaders/simple_textured_shadows.vert",
            "data/shaders/simple_textured_shadows.frag",
            "data/shaders/common"
        )};

        add_shader(shader);

        auto material {cache_material.load("simple_textured_shadows"_H, shader)};
        material->add_texture("u_material.ambient_diffuse"_H);
        material->add_uniform(bb::Material::Uniform::Vec3, "u_material.specular"_H);
        material->add_uniform(bb::Material::Uniform::Float, "u_material.shininess"_H);
    }
}

void LevelScene::load_platform() {
    auto mesh {std::make_shared<bb::Mesh>(
        "data/models/platform.obj",
        "Platform",
        bb::Mesh::Type::PTN
    )};

    auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
        mesh->get_vertices(),
        mesh->get_vertices_size()
    )};

    auto index_buffer {std::make_shared<bb::IndexBuffer>(
        mesh->get_indices(),
        mesh->get_indices_size()
    )};

    auto vertex_array {cache_vertex_array.load("platform"_H)};
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Float, 3);
        layout.add(1, bb::VertexBufferLayout::Float, 2);
        layout.add(2, bb::VertexBufferLayout::Float, 3);

        va->add_vertex_buffer(vertex_buffer, layout);
        va->add_index_buffer(index_buffer);
    });

    bb::TextureSpecification specification;
    specification.mipmap_levels = 2;
    auto texture {cache_texture.load("platform"_H, "data/textures/wood-bare.png", specification)};

    auto material_instance {cache_material_instance.load("platform"_H, cache_material["simple_textured_shadows"_H])};
    material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.25f));
    material_instance->set_float("u_material.shininess"_H, 16.0f);
    material_instance->flags |= bb::Material::CastShadow;
}

void LevelScene::load_ball() {
    auto mesh {std::make_shared<bb::Mesh>(
        "data/models/ball.obj",
        "Sphere",
        bb::Mesh::Type::PTN
    )};

    auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
        mesh->get_vertices(),
        mesh->get_vertices_size()
    )};

    auto index_buffer {std::make_shared<bb::IndexBuffer>(
        mesh->get_indices(),
        mesh->get_indices_size()
    )};

    auto vertex_array {cache_vertex_array.load("ball"_H)};
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Float, 3);
        layout.add(1, bb::VertexBufferLayout::Float, 2);
        layout.add(2, bb::VertexBufferLayout::Float, 3);

        va->add_vertex_buffer(vertex_buffer, layout);
        va->add_index_buffer(index_buffer);
    });

    bb::TextureSpecification specification;
    auto texture {cache_texture.load("ball"_H, "data/textures/ball-texture.png", specification)};

    auto material_instance {cache_material_instance.load("ball"_H, cache_material["simple_textured_shadows"_H])};
    material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.75f));
    material_instance->set_float("u_material.shininess"_H, 64.0f);
    material_instance->flags |= bb::Material::CastShadow;
}

void LevelScene::load_paddle() {
    auto mesh {std::make_shared<bb::Mesh>(
        "data/models/paddle.obj",
        "Paddle",
        bb::Mesh::Type::PTN
    )};

    auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
        mesh->get_vertices(),
        mesh->get_vertices_size()
    )};

    auto index_buffer {std::make_shared<bb::IndexBuffer>(
        mesh->get_indices(),
        mesh->get_indices_size()
    )};

    auto vertex_array {cache_vertex_array.load("paddle"_H)};
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Float, 3);
        layout.add(1, bb::VertexBufferLayout::Float, 2);
        layout.add(2, bb::VertexBufferLayout::Float, 3);

        va->add_vertex_buffer(vertex_buffer, layout);
        va->add_index_buffer(index_buffer);
    });

    bb::TextureSpecification specification;
    auto texture {cache_texture.load("paddle"_H, "data/textures/paddle-texture.png", specification)};

    auto material_instance {cache_material_instance.load("paddle"_H, cache_material["simple_textured_shadows"_H])};
    material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.4f));
    material_instance->set_float("u_material.shininess"_H, 32.0f);
    material_instance->flags |= bb::Material::CastShadow;
}

void LevelScene::load_brick() {
    auto mesh {std::make_shared<bb::Mesh>(
        "data/models/brick.obj",
        "Brick",
        bb::Mesh::Type::PTN
    )};

    auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
        mesh->get_vertices(),
        mesh->get_vertices_size()
    )};

    auto index_buffer {std::make_shared<bb::IndexBuffer>(
        mesh->get_indices(),
        mesh->get_indices_size()
    )};

    auto vertex_array {cache_vertex_array.load("brick"_H)};
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Float, 3);
        layout.add(1, bb::VertexBufferLayout::Float, 2);
        layout.add(2, bb::VertexBufferLayout::Float, 3);

        va->add_vertex_buffer(vertex_buffer, layout);
        va->add_index_buffer(index_buffer);
    });

    bb::TextureSpecification specification;
    cache_texture.load("brick1"_H, "data/textures/brick-texture1.png", specification);
    cache_texture.load("brick2"_H, "data/textures/brick-texture2.png", specification);
    cache_texture.load("brick3"_H, "data/textures/brick-texture3.png", specification);

    {
        auto material_instance {cache_material_instance.load("brick1"_H, cache_material["simple_textured_shadows"_H])};
        material_instance->set_texture("u_material.ambient_diffuse"_H, cache_texture["brick1"_H], 0);
        material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.5f));
        material_instance->set_float("u_material.shininess"_H, 32.0f);
        material_instance->flags |= bb::Material::CastShadow;
    }

    {
        auto material_instance {cache_material_instance.load("brick2"_H, cache_material["simple_textured_shadows"_H])};
        material_instance->set_texture("u_material.ambient_diffuse"_H, cache_texture["brick2"_H], 0);
        material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.5f));
        material_instance->set_float("u_material.shininess"_H, 32.0f);
        material_instance->flags |= bb::Material::CastShadow;
    }

    {
        auto material_instance {cache_material_instance.load("brick3"_H, cache_material["simple_textured_shadows"_H])};
        material_instance->set_texture("u_material.ambient_diffuse"_H, cache_texture["brick3"_H], 0);
        material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.5f));
        material_instance->set_float("u_material.shininess"_H, 32.0f);
        material_instance->flags |= bb::Material::CastShadow;
    }
}

void LevelScene::load_lamp() {
    {
        auto mesh {std::make_shared<bb::Mesh>(
            "data/models/lamp.obj",
            "Stand",
            bb::Mesh::Type::PTN
        )};

        auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
            mesh->get_vertices(),
            mesh->get_vertices_size()
        )};

        auto index_buffer {std::make_shared<bb::IndexBuffer>(
            mesh->get_indices(),
            mesh->get_indices_size()
        )};

        auto vertex_array {cache_vertex_array.load("lamp_stand"_H)};
        vertex_array->configure([&](bb::VertexArray* va) {
            bb::VertexBufferLayout layout;
            layout.add(0, bb::VertexBufferLayout::Float, 3);
            layout.add(1, bb::VertexBufferLayout::Float, 2);
            layout.add(2, bb::VertexBufferLayout::Float, 3);

            va->add_vertex_buffer(vertex_buffer, layout);
            va->add_index_buffer(index_buffer);
        });
    }

    {
        auto mesh {std::make_shared<bb::Mesh>(
            "data/models/lamp.obj",
            "Bulb",
            bb::Mesh::Type::P
        )};

        auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
            mesh->get_vertices(),
            mesh->get_vertices_size()
        )};

        auto index_buffer {std::make_shared<bb::IndexBuffer>(
            mesh->get_indices(),
            mesh->get_indices_size()
        )};

        auto vertex_array {cache_vertex_array.load("lamp_bulb"_H)};
        vertex_array->configure([&](bb::VertexArray* va) {
            bb::VertexBufferLayout layout;
            layout.add(0, bb::VertexBufferLayout::Float, 3);

            va->add_vertex_buffer(vertex_buffer, layout);
            va->add_index_buffer(index_buffer);
        });
    }

    bb::TextureSpecification specification;
    auto texture {cache_texture.load("lamp"_H, "data/textures/lamp-texture.png", specification)};

    {
        auto material_instance {cache_material_instance.load("lamp_stand"_H, cache_material["simple_textured_shadows"_H])};
        material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
        material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.5f));
        material_instance->set_float("u_material.shininess"_H, 64.0f);
        material_instance->flags |= bb::Material::CastShadow;
    }

    {
        auto material_instance {cache_material_instance.load("lamp_bulb"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, glm::vec3(1.0f));
    }
}

void LevelScene::load_orb() {
    auto mesh {std::make_shared<bb::Mesh>(
        "data/models/ball.obj",
        "Sphere",
        bb::Mesh::Type::P
    )};

    auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
        mesh->get_vertices(),
        mesh->get_vertices_size()
    )};

    auto index_buffer {std::make_shared<bb::IndexBuffer>(
        mesh->get_indices(),
        mesh->get_indices_size()
    )};

    auto vertex_array {cache_vertex_array.load("orb"_H)};
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Float, 3);

        va->add_vertex_buffer(vertex_buffer, layout);
        va->add_index_buffer(index_buffer);
    });

    {
        auto material_instance {cache_material_instance.load("orb0"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, ORB_COLORS[static_cast<int>(OrbType::SpeedUp)]);
    }

    {
        auto material_instance {cache_material_instance.load("orb1"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, ORB_COLORS[static_cast<int>(OrbType::SpeedDown)]);
    }

    {
        auto material_instance {cache_material_instance.load("orb2"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, ORB_COLORS[static_cast<int>(OrbType::ExtraLife)]);
    }

    {
        auto material_instance {cache_material_instance.load("orb3"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, ORB_COLORS[static_cast<int>(OrbType::Die)]);
    }

    {
        auto material_instance {cache_material_instance.load("orb4"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, ORB_COLORS[static_cast<int>(OrbType::FireBall)]);
    }

    {
        auto material_instance {cache_material_instance.load("orb5"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, ORB_COLORS[static_cast<int>(OrbType::NormalBall)]);
    }

    {
        auto material_instance {cache_material_instance.load("orb6"_H, cache_material["flat"_H])};
        material_instance->set_vec3("u_material.color"_H, ORB_COLORS[static_cast<int>(OrbType::ExtraBall)]);
    }
}

void LevelScene::update_collisions() {
    for (auto& [index, ball] : balls) {
        Sphere s;
        s.position = ball.get_position();
        s.radius = ball.radius;

        Box b;
        b.position = paddle.get_position();
        b.width = paddle.get_dimensions().x;
        b.height = paddle.get_dimensions().y;
        b.depth = paddle.get_dimensions().z;

        if (collision_sphere_box(s, b)) {
            bb::log_message("Collided!\n");

            const SphereBoxSide side {sphere_box_side_2d(s, b)};

            enqueue_event<BallPaddleCollisionEvent>(index, side);
        }
    }

    for (const auto& [ball_index, ball] : balls) {
        for (const auto& [brick_index, brick] : bricks) {
            Sphere s;
            s.position = ball.get_position();
            s.radius = ball.radius;

            Box b;
            b.position = brick.get_position();
            b.width = brick.get_dimensions().x;
            b.height = brick.get_dimensions().y;
            b.depth = brick.get_dimensions().z;

            if (collision_sphere_box(s, b)) {
                bb::log_message("Collided!\n");

                const SphereBoxSide side {sphere_box_side_2d(s, b)};

                enqueue_event<BallBrickCollisionEvent>(ball_index, brick_index, side);
            }
        }
    }

    for (auto& [index, orb] : orbs) {
        Sphere s;
        s.position = orb.position;
        s.radius = orb.radius;

        Box b;
        b.position = paddle.get_position();
        b.width = paddle.get_dimensions().x;
        b.height = paddle.get_dimensions().y;
        b.depth = paddle.get_dimensions().z;

        if (collision_sphere_box(s, b)) {
            bb::log_message("Collided!\n");

            enqueue_event<OrbPaddleCollisionEvent>(index);
        }
    }
}

void LevelScene::update_bricks() {
    for (auto& [_, brick] : bricks) {
        if (brick.get_grid().y == 0) {
            continue;
        }

        for (const auto& [_, brick_below] : bricks) {
            // Check if there is a brick below
            if (brick_below.get_grid().y == brick.get_grid().y - 1) {
                if (brick_below.get_grid().x == brick.get_grid().x && brick_below.get_grid().z == brick.get_grid().z) {
                    goto continue_outer;
                }
            }
        }

        // This brick is in the air :P
        brick.lower_grid();
        brick.set_position(brick.get_grid());

        continue_outer:
        continue;
    }
}

void LevelScene::update_paddle(Paddle& paddle) {
    paddle.set_position(paddle.get_position().x + paddle.velocity_x * get_delta());

    static constexpr float min {PLATFORM_EDGE_MIN_X + 0.7f};
    static constexpr float max {PLATFORM_EDGE_MAX_X - 0.7f};

    if (paddle.get_position().x < min) {
        paddle.set_position(min);
    }

    if (paddle.get_position().x > max) {
        paddle.set_position(max);
    }
}

void LevelScene::update_orb(Orb& orb) {
    orb.position += orb.velocity * get_delta();

    if (orb.position.z > DEADLINE_Z) {
        enqueue_event<OrbMissEvent>(orb.get_index());
    }
}

void LevelScene::update_ball(Ball& ball) {
    ball.set_position_x((ball.get_position() + ball.velocity * get_delta()).x);
    ball.set_position_z((ball.get_position() + ball.velocity * get_delta()).z);

    if (ball.attached_to_paddle) {
        // Override position
        ball.set_position_x(paddle.get_position().x);
        ball.set_position_z(paddle.get_position().z - 1.0f);
    }

    const auto perpendicular_velocity {glm::rotate(glm::normalize(ball.velocity), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))};
    const float current_rotation {rotate_ball(ball)};

    glm::mat4 trans {glm::mat4(1.0f)};
    trans = glm::translate(trans, ball.get_position());
    trans *= glm::toMat4(ball.rotation);
    trans = glm::rotate(trans, current_rotation, perpendicular_velocity);
    trans = glm::scale(trans, glm::vec3(ball.radius));  // Default ball size should be 1 meter in radius, so radius is scale

    ball.transformation = trans;

    const glm::quat rot {glm::angleAxis(current_rotation, perpendicular_velocity)};
    ball.rotation = rot * ball.rotation;

    auto& data {user_data<Data>()};

    if (ball.get_position().x > PLATFORM_EDGE_MAX_X - ball.radius) {
        ball.set_position_x(PLATFORM_EDGE_MAX_X - ball.radius);
        ball.velocity.x *= -1.0f;

        play_sound(data.sound_collision_wall);
    }

    if (ball.get_position().x < PLATFORM_EDGE_MIN_X + ball.radius) {
        ball.set_position_x(PLATFORM_EDGE_MIN_X + ball.radius);
        ball.velocity.x *= -1.0f;

        play_sound(data.sound_collision_wall);
    }

    if (ball.get_position().z < PLATFORM_EDGE_MIN_Z + ball.radius) {
        ball.set_position_z(PLATFORM_EDGE_MIN_Z + ball.radius);
        ball.velocity.z *= -1.0f;

        play_sound(data.sound_collision_wall);
    }

    if (ball.get_position().z > DEADLINE_Z) {
        enqueue_event<BallMissEvent>(ball.get_index());
    }
}

void LevelScene::shoot_balls() {
    bool any_ball {false};

    for (auto& [_, ball] : balls) {
        if (ball.attached_to_paddle) {
            const auto vector {glm::vec3(glm::linearRand(-5.0f, 5.0f), 0.0f, -5.0f)};
            const auto direction {glm::normalize(vector)};

            ball.velocity = direction * SHOOT_VELOCITY;
            ball.attached_to_paddle = false;

            any_ball = true;
        }
    }

    if (any_ball) {
        auto& data {user_data<Data>()};
        play_sound(data.sound_collision_paddle);
    }
}

void LevelScene::create_ball() {
    const auto index {id_gen.generate()};
    balls[index] = Ball(index);
}

void LevelScene::spawn_orb(glm::vec3 position) {
    const auto velocity {glm::vec3(0.0f, 0.0f, glm::linearRand(5.0f, 7.0f))};
    const float random_type {glm::linearRand(static_cast<float>(OrbType::FIRST), static_cast<float>(OrbType::LAST) + 1.0f)};

    const auto index {id_gen.generate()};
    orbs[index] = Orb(index, position.x, position.z, velocity, static_cast<OrbType>(static_cast<int>(random_type)));
}

void LevelScene::die() {
    auto& data {user_data<Data>()};
    play_sound(data.sound_die);

    lives--;
    balls.clear();
    orbs.clear();

    if (lives == 0) {
        lose();
    } else {
        paddle = Paddle();
        create_ball();
    }
}

void LevelScene::win() {
    game_over = GameOver::Won;
    balls.clear();

    auto& data {user_data<Data>()};
    play_sound(data.sound_won);

    bb::log_message("Congratulations!\n");
}

void LevelScene::lose() {
    game_over = GameOver::Lost;

    auto& data {user_data<Data>()};
    play_sound(data.sound_lost);

    bb::log_message("Game over!\n");
}

std::optional<std::unordered_map<unsigned int, Brick>> LevelScene::load_level(const std::string& file_path, IdGenerator& gen) {
    std::ifstream file {file_path};

    if (!file.is_open()) {
        return std::nullopt;
    }

    std::unordered_map<unsigned int, Brick> result;

    const nlohmann::json root = nlohmann::json::parse(file);

    try {
        const nlohmann::json& j_bricks {root["bricks"].get<nlohmann::json>()};

        for (const nlohmann::json& j_brick : j_bricks) {
            const nlohmann::json& position {j_brick["position"].get<nlohmann::json>()};
            const int type {j_brick["type"].get<int>()};

            if (position.size() != 3u) {
                return std::nullopt;
            }

            const int x {position[0].get<int>()};
            const int y {position[1].get<int>()};
            const int z {position[2].get<int>()};

            if (x < BRICKS_GRID_MIN_X || x > BRICKS_GRID_MAX_X) {
                return std::nullopt;
            }

            if (z < BRICKS_GRID_MIN_Z || z > BRICKS_GRID_MAX_Z) {
                return std::nullopt;
            }

            if (y < BRICKS_GRID_MIN_Y || y > BRICKS_GRID_MAX_Y) {
                return std::nullopt;
            }

            if (type < static_cast<int>(BrickType::FIRST) || type > static_cast<int>(BrickType::LAST)) {
                return std::nullopt;
            }

            const auto index {gen.generate()};
            result[index] = Brick(index, glm::ivec3(x, y, z), static_cast<BrickType>(type));
        }
    } catch (const nlohmann::json::exception&) {
        return std::nullopt;
    }

    return std::make_optional(result);
}

glm::vec2 LevelScene::bounce_ball_off_paddle(const Ball& ball) {
    // https://www.mathsisfun.com/polar-cartesian-coordinates.html

    const float original_velocity {glm::length(ball.velocity)};

    const float distance {glm::abs(paddle.get_position().x - ball.get_position().x)};
    const float theta {mapf(distance, 0.0f, paddle.get_dimensions().x, 0.0f, 70.0f)};

    const float directed_theta {paddle.get_position().x - ball.get_position().x > 0.0f ? theta + 90.0f : -theta + 90.0f};

    return glm::vec2(original_velocity * glm::cos(glm::radians(directed_theta)), original_velocity * glm::sin(glm::radians(directed_theta)));
}

float LevelScene::rotate_ball(const Ball& ball) {
    const float velocity {glm::length(ball.velocity) * get_delta()};
    const float half_diameter {glm::pi<float>() * ball.radius};
    const float theta {velocity / half_diameter};  // Radians

    return theta;
}

void LevelScene::on_ball_paddle_collision(const BallPaddleCollisionEvent& event) {
    if (balls.find(event.ball_index) != balls.end()) {
        Ball& ball {balls.at(event.ball_index)};

        switch (event.side) {
            case SphereBoxSide::Back: {
                const auto velocity {bounce_ball_off_paddle(ball)};

                ball.set_position_z(paddle.get_position().z - paddle.get_dimensions().z - ball.radius);
                ball.velocity.z = -velocity.y;
                ball.velocity.x = velocity.x;

                break;
            }
            case SphereBoxSide::Left:
                ball.set_position_x(paddle.get_position().x - paddle.get_dimensions().x - ball.radius);
                ball.velocity.x *= -1;
                break;
            case SphereBoxSide::Right:
                ball.set_position_x(paddle.get_position().x + paddle.get_dimensions().x + ball.radius);
                ball.velocity.x *= -1;
                break;
            default:
                break;
        }
    }

    auto& data {user_data<Data>()};
    play_sound(data.sound_collision_paddle);
}

void LevelScene::on_ball_miss(const BallMissEvent& event) {
    balls.erase(event.ball_index);

    if (balls.empty()) {
        die();
    }
}

void LevelScene::on_orb_miss(const OrbMissEvent& event) {
    orbs.erase(event.orb_index);
}

void LevelScene::on_orb_paddle_collision(const OrbPaddleCollisionEvent& event) {
    if (orbs.find(event.orb_index) != orbs.end()) {
        Orb& orb {orbs.at(event.orb_index)};

        auto& data {user_data<Data>()};
        play_sound(data.sound_switch);

        switch (orb.get_type()) {
            case OrbType::SpeedUp:
                for (auto& [_, ball] : balls) {
                    ball.velocity *= 1.25f;
                }

                break;
            case OrbType::SpeedDown:
                for (auto& [_, ball] : balls) {
                    ball.velocity *= 0.8f;
                }

                break;
            case OrbType::ExtraLife:
                lives++;

                break;
            case OrbType::Die:
                die();

                break;
            case OrbType::FireBall:
                for (auto& [_, ball] : balls) {
                    ball.fire = true;
                    ball.radius = BALL_RADIUS_FIRE;
                }

                break;
            case OrbType::NormalBall:
                for (auto& [_, ball] : balls) {
                    ball.fire = false;
                    ball.radius = BALL_RADIUS_NORMAL;
                }

                break;
            case OrbType::ExtraBall:
                create_ball();

                break;
            default:
                break;
        }

        score += orb.get_points();

        // It could have been already erased by die()
        if (orbs.find(event.orb_index) != orbs.end()) {
            orbs.erase(event.orb_index);
        }
    }
}

void LevelScene::on_ball_brick_collision(const BallBrickCollisionEvent& event) {
    Ball& ball {balls.at(event.ball_index)};  // Can't fail

    if (bricks.find(event.brick_index) != bricks.end()) {
        const Brick& brick {bricks.at(event.brick_index)};

        if (!ball.fire) {
            switch (event.side) {
                case SphereBoxSide::Front:
                    ball.set_position_z(brick.get_position().z + brick.get_dimensions().z + ball.radius);
                    ball.velocity.z *= -1.0f;
                    break;
                case SphereBoxSide::Back:
                    ball.set_position_z(brick.get_position().z - brick.get_dimensions().z - ball.radius);
                    ball.velocity.z *= -1.0f;
                    break;
                case SphereBoxSide::Left:
                    ball.set_position_x(brick.get_position().x - brick.get_dimensions().x - ball.radius);
                    ball.velocity.x *= -1.0f;
                    break;
                case SphereBoxSide::Right:
                    ball.set_position_x(brick.get_position().x + brick.get_dimensions().x + ball.radius);
                    ball.velocity.x *= -1.0f;
                    break;
            }
        }

        score += brick.get_points();
        bricks.erase(event.brick_index);

        auto& data {user_data<Data>()};
        play_sound(data.sound_collision_brick);

        if (glm::linearRand(0.0f, 1.0f) > ORB_RATE) {
            spawn_orb(brick.get_position());
        }

        if (bricks.empty()) {
            win();
        }
    }
}

void LevelScene::draw_bounding_box(const Box& box) {
    const auto& position {box.position};
    const auto width {glm::vec3(1.0f, 0.0f, 0.0f) * box.width};
    const auto height {glm::vec3(0.0f, 1.0f, 0.0f) * box.height};
    const auto depth {glm::vec3(0.0f, 0.0f, 1.0f) * box.depth};

    debug_add_line(position - width - depth - height, position + width - depth - height, GREEN);
    debug_add_line(position - width - depth - height, position - width + depth - height, GREEN);
    debug_add_line(position - width + depth - height, position + width + depth - height, GREEN);
    debug_add_line(position + width + depth - height, position + width - depth - height, GREEN);

    debug_add_line(position - width - depth + height, position + width - depth + height, GREEN);
    debug_add_line(position - width - depth + height, position - width + depth + height, GREEN);
    debug_add_line(position - width + depth + height, position + width + depth + height, GREEN);
    debug_add_line(position + width + depth + height, position + width - depth + height, GREEN);

    debug_add_line(position - width - depth - height, position - width - depth + height, GREEN);
    debug_add_line(position + width - depth - height, position + width - depth + height, GREEN);
    debug_add_line(position + width + depth - height, position + width + depth + height, GREEN);
    debug_add_line(position - width + depth - height, position - width + depth + height, GREEN);
}

void LevelScene::draw_fps() {
    auto& data {user_data<Data>()};

    bb::Text text;
    text.font = data.basic_font;
    text.string = std::to_string(get_fps()) + " FPS, " + std::to_string(get_delta()) + " ms";
    text.position = glm::vec2(2.0f, 2.0f);
    text.color = glm::vec3(0.9f);
    text.scale = 0.3f;
    add_text(text);
}
