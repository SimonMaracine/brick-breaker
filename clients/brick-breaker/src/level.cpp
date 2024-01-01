#include <unordered_map>

#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "my_camera_controller.hpp"
#include "collision.hpp"
#include "paddle.hpp"
#include "ball.hpp"
#include "events.hpp"
#include "level.hpp"

// https://www.goodtextures.com/image/21296/old-bricks
// https://stackoverflow.com/questions/39280104/how-to-get-current-camera-position-from-view-matrix

using namespace resmanager::literals;

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

    directional_light.direction = glm::vec3(-0.6f, -1.0f, 0.2f);
    directional_light.ambient_color = glm::vec3(0.1f);
    directional_light.diffuse_color = glm::vec3(1.0f);
    directional_light.specular_color = glm::vec3(1.0f);

    connect_event<bb::WindowResizedEvent, &LevelScene::on_window_resized>(this);
    connect_event<bb::KeyPressedEvent, &LevelScene::on_key_pressed>(this);
    connect_event<bb::KeyReleasedEvent, &LevelScene::on_key_released>(this);

    connect_event<BallPaddleCollisionEvent, &LevelScene::on_ball_paddle_collision>(this);

    load_shaders();
    load_platform();
    load_ball();
    load_paddle();
    load_brick();

    paddle = Paddle();
    balls.clear();
    balls[0u] = Ball(0u);
}

void LevelScene::on_exit() {

}

void LevelScene::on_update() {
    cam_controller.update_controls(get_delta());
    cam_controller.update_camera(get_delta());

    capture(cam, cam_controller.get_position());
    add_light(directional_light);

    update_paddle(paddle);

    for (auto& [_, ball] : balls) {
        update_ball(ball);
    }

    update_collisions();

    bb::Renderable r_platform;
    r_platform.vertex_array = cache_vertex_array["platform"_H];
    r_platform.material = cache_material_instance["platform"_H];
    r_platform.rotation.y = glm::radians(90.0f);
    add_renderable(r_platform);

    bb::Renderable r_paddle;
    r_paddle.vertex_array = cache_vertex_array["paddle"_H];
    r_paddle.material = cache_material_instance["paddle"_H];
    r_paddle.position = paddle.get_position();
    r_paddle.rotation = paddle.get_rotation();
    r_paddle.scale = paddle.get_scale();
    add_renderable(r_paddle);

    Box b_paddle;
    b_paddle.position = paddle.get_position();
    b_paddle.width = paddle.get_dimensions().x;
    b_paddle.height = paddle.get_dimensions().y;
    b_paddle.depth = paddle.get_dimensions().z;
    draw_bounding_box(b_paddle);

    for (const auto& [_, ball] : balls) {
        bb::Renderable r_ball;
        r_ball.vertex_array = cache_vertex_array["ball"_H];
        r_ball.material = cache_material_instance["ball"_H];
        r_ball.transformation = ball.transformation;
        add_renderable(r_ball);

        debug_add_line(ball.position, ball.position + glm::vec3(ball.size, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        debug_add_line(ball.position, ball.position - glm::vec3(ball.size, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        debug_add_line(ball.position, ball.position + glm::vec3(0.0f, 0.0f, ball.size), glm::vec3(0.0f, 1.0f, 0.0f));
        debug_add_line(ball.position, ball.position - glm::vec3(0.0f, 0.0f, ball.size), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    bb::Renderable r_brick;
    r_brick.vertex_array = cache_vertex_array["brick1"_H];
    r_brick.material = cache_material_instance["brick1"_H];
    r_brick.position = glm::vec3(2.0f, 0.65f, 0.5f);
    r_brick.scale = 0.25f;
    add_renderable(r_brick);

    debug_add_line(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void LevelScene::on_window_resized(bb::WindowResizedEvent& event) {
    cam.set_projection_matrix(event.width, event.height, LENS_FOV, LENS_NEAR, LENS_FAR);
}

void LevelScene::on_key_pressed(bb::KeyPressedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_LEFT:
            paddle.velocity = -11.0f;
            break;
        case bb::KeyCode::K_RIGHT:
            paddle.velocity = 11.0f;
            break;
        case bb::KeyCode::K_i:
            balls[0].position.z -= 0.09f;
            break;
        case bb::KeyCode::K_k:
            balls[0].position.z += 0.09f;
            break;
        case bb::KeyCode::K_j:
            balls[0].position.x -= 0.09f;
            break;
        case bb::KeyCode::K_l:
            balls[0].position.x += 0.09f;
            break;
        default:
            break;
    }
}

void LevelScene::on_key_released(bb::KeyReleasedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_LEFT:
        case bb::KeyCode::K_RIGHT:
            paddle.velocity = 0.0f;
            break;
        case bb::KeyCode::K_r:
            change_scene("level");
            break;
        default:
            break;
    }
}

void LevelScene::load_shaders() {
    {
        // This is a generic shader
        auto shader {std::make_shared<bb::Shader>(
            "data/shaders/simple_textured.vert",
            "data/shaders/simple_textured.frag"
        )};

        add_shader(shader);

        // And a generic material
        auto material {cache_material.load("simple_textured"_H, shader)};
        material->add_texture("u_material.ambient_diffuse"_H);
        material->add_uniform(bb::Material::Uniform::Vec3, "u_material.specular"_H);
        material->add_uniform(bb::Material::Uniform::Float, "u_material.shininess"_H);
    }

    {
        // This is also a generic shader
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
    specification.format = bb::Format::Rgb8;
    specification.mipmap_levels = 3;
    auto texture {cache_texture.load("platform"_H, "data/textures/old-bricks.jpeg", specification)};

    auto material_instance {cache_material_instance.load("platform"_H, cache_material["simple_textured"_H])};
    material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.35f));
    material_instance->set_float("u_material.shininess"_H, 16.0f);
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

    auto material_instance {cache_material_instance.load("ball"_H, cache_material["simple_textured"_H])};
    material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.7f));
    material_instance->set_float("u_material.shininess"_H, 64.0f);
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

    auto material_instance {cache_material_instance.load("paddle"_H, cache_material["simple_textured"_H])};
    material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.4f));
    material_instance->set_float("u_material.shininess"_H, 32.0f);
}

void LevelScene::load_brick() {
    auto mesh {std::make_shared<bb::Mesh>(
        "data/models/brick1.obj",
        "Brick1",
        bb::Mesh::Type::PN
    )};

    auto vertex_buffer {std::make_shared<bb::VertexBuffer>(
        mesh->get_vertices(),
        mesh->get_vertices_size()
    )};

    auto index_buffer {std::make_shared<bb::IndexBuffer>(
        mesh->get_indices(),
        mesh->get_indices_size()
    )};

    auto vertex_array {cache_vertex_array.load("brick1"_H)};
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Float, 3);
        layout.add(1, bb::VertexBufferLayout::Float, 3);

        va->add_vertex_buffer(vertex_buffer, layout);
        va->add_index_buffer(index_buffer);
    });

    auto material_instance {cache_material_instance.load("brick1"_H, cache_material["simple"_H])};
    material_instance->set_vec3("u_material.ambient_diffuse"_H, glm::vec3(0.7f, 0.7f, 0.8f));
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.4f));
    material_instance->set_float("u_material.shininess"_H, 32.0f);
}

void LevelScene::update_collisions() {
    for (auto& [index, ball] : balls) {
        Sphere s;
        s.position = ball.position;
        s.radius = ball.size;

        Box b;
        b.position = paddle.get_position();
        b.width = paddle.get_dimensions().x;
        b.height = paddle.get_dimensions().y;
        b.depth = paddle.get_dimensions().z;

        if (collision_sphere_box(s, b)) {
            bb::log_message("Collided!\n");

            enqueue_event<BallPaddleCollisionEvent>(index);
        }
    }
}

void LevelScene::update_paddle(Paddle& paddle) {
    paddle.position += paddle.velocity * get_delta();

    if (paddle.position < -10.0f) {
        paddle.position = -10.0f;
    }

    if (paddle.position > 10.0f) {
        paddle.position = 10.0f;
    }
}

void LevelScene::update_ball(Ball& ball) {
    // ball.position += ball.velocity * get_delta();

    const auto perpendicular_velocity {glm::rotate(glm::normalize(ball.velocity), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))};

    glm::mat4 trans {glm::mat4(1.0f)};
    trans = glm::translate(trans, ball.position);
    trans *= glm::toMat4(ball.rotation);
    trans = glm::rotate(trans, glm::length(ball.velocity) * 0.1f, perpendicular_velocity);
    trans = glm::scale(trans, glm::vec3(ball.size));  // Default ball size should be 1 meter in radius, so size is scale

    ball.transformation = trans;

    const glm::quat rot {glm::angleAxis(glm::length(ball.velocity) * 0.01f, perpendicular_velocity)};
    ball.rotation = rot * ball.rotation;

    if (ball.position.x < -9.3f || ball.position.x > 9.3f) {
        ball.velocity.x = -ball.velocity.x;
    }

    if (ball.position.z < -9.3f) {
        ball.velocity.z = -ball.velocity.z;
    }

    if (ball.position.z > 10.0f) {
        // bb::log_message("Game over!\n");  // TODO
    }
}

void LevelScene::on_ball_paddle_collision(BallPaddleCollisionEvent& event) {
    Ball& ball {balls[event.ball_index]};  // TODO can fail

    ball.position.z = paddle.get_position().z - paddle.get_dimensions().z - ball.size;
    ball.velocity.z *= -1.0f;
}

void LevelScene::draw_bounding_box(const Box& box) {
    static constexpr auto color {glm::vec3(0.0f, 1.0f, 0.0f)};
    const auto& position {box.position};
    const auto width {glm::vec3(1.0f, 0.0f, 0.0f) * box.width};
    const auto height {glm::vec3(0.0f, 1.0f, 0.0f) * box.height};
    const auto depth {glm::vec3(0.0f, 0.0f, 1.0f) * box.depth};

    debug_add_line(position - width - depth - height, position + width - depth - height, color);
    debug_add_line(position - width - depth - height, position - width + depth - height, color);
    debug_add_line(position - width + depth - height, position + width + depth - height, color);
    debug_add_line(position + width + depth - height, position + width - depth - height, color);

    debug_add_line(position - width - depth + height, position + width - depth + height, color);
    debug_add_line(position - width - depth + height, position - width + depth + height, color);
    debug_add_line(position - width + depth + height, position + width + depth + height, color);
    debug_add_line(position + width + depth + height, position + width - depth + height, color);

    debug_add_line(position - width - depth - height, position - width - depth + height, color);
    debug_add_line(position + width - depth - height, position + width - depth + height, color);
    debug_add_line(position + width + depth - height, position + width + depth + height, color);
    debug_add_line(position - width + depth - height, position - width + depth + height, color);
}
