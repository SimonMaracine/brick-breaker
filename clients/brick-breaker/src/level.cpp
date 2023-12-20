#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "my_camera_controller.hpp"
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

    directional_light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    directional_light.ambient_color = glm::vec3(0.1f);
    directional_light.diffuse_color = glm::vec3(1.0f);
    directional_light.specular_color = glm::vec3(1.0f);

    connect_event<bb::WindowResizedEvent, &LevelScene::on_window_resized>(this);
    connect_event<bb::KeyPressedEvent, &LevelScene::on_key_pressed>(this);
    connect_event<bb::KeyReleasedEvent, &LevelScene::on_key_released>(this);

    load_shaders();
    load_platform();
    load_ball();
    load_paddle();

    ball_position = glm::vec3(0.0f, 0.65f, 0.0f);
    ball_velocity = glm::linearRand(glm::vec3(2.0f, 0.0f, 4.0f), glm::vec3(1.0f, 0.0f, 3.0f));
    ball_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void LevelScene::on_exit() {

}

void LevelScene::on_update() {
    cam_controller.update_controls(get_delta());
    cam_controller.update_camera(get_delta());

    capture(cam, cam_controller.get_position());
    add_light(directional_light);

    bb::Renderable platform;
    platform.vertex_array = cache_vertex_array["platform"_H];
    platform.material = cache_material_instance["platform"_H];
    platform.rotation.y += glm::radians(90.0f);

    add_renderable(platform);

    if (ball_position.x < -9.3f || ball_position.x > 9.3f) {
        ball_velocity.x = -ball_velocity.x;
    }

    if (ball_position.z < -9.3f) {
        ball_velocity.z = -ball_velocity.z;
    }

    if (ball_position.z > 9.8f && ball_position.z < 11.0f) {
        if (ball_position.x > paddle_position - 2.0f && ball_position.x < paddle_position + 2.0f) {
            ball_position.z = 9.8f - 0.001f;
            ball_velocity.z = -ball_velocity.z;
        }
    }

    if (ball_position.z > 10.0f) {
        bb::log_message("Game over!\n");
    }

    ball_position += ball_velocity * get_delta();
    paddle_position += paddle_velocity * get_delta();

    glm::mat4 t {glm::mat4(1.0f)};
    t = glm::translate(t, ball_position);
    t *= glm::toMat4(ball_rotation);
    t = glm::rotate(t, glm::length(ball_velocity) * 0.1f, glm::rotate(ball_velocity, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    t = glm::scale(t, glm::vec3(0.35f));

    const glm::quat r {glm::angleAxis(glm::length(ball_velocity) * 0.01f, glm::rotate(glm::normalize(ball_velocity), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)))};
    ball_rotation = r * ball_rotation;

    bb::Renderable ball;
    ball.vertex_array = cache_vertex_array["ball"_H];
    ball.material = cache_material_instance["ball"_H];
    ball.transformation = t;

    add_renderable(ball);

    bb::Renderable paddle;
    paddle.vertex_array = cache_vertex_array["paddle"_H];
    paddle.material = cache_material_instance["paddle"_H];
    paddle.position = glm::vec3(paddle_position, 0.7f, 10.5f);
    paddle.rotation.y = glm::radians(90.0f);
    paddle.scale = 0.35f;

    add_renderable(paddle);

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
            paddle_velocity = -11.0f;
            break;
        case bb::KeyCode::K_RIGHT:
            paddle_velocity = 11.0f;
            break;
        default:
            break;
    }
}

void LevelScene::on_key_released(bb::KeyReleasedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_LEFT:
        case bb::KeyCode::K_RIGHT:
            paddle_velocity = 0.0f;
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
    specification.mipmap_levels = 1;
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

    auto vertex_array {cache_vertex_array.load("paddle"_H)};
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Float, 3);
        layout.add(1, bb::VertexBufferLayout::Float, 3);

        va->add_vertex_buffer(vertex_buffer, layout);
        va->add_index_buffer(index_buffer);
    });

    auto material_instance {cache_material_instance.load("paddle"_H, cache_material["simple"_H])};
    material_instance->set_vec3("u_material.ambient_diffuse"_H, glm::vec3(0.64f, 0.6f, 0.65f));
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.4f));
    material_instance->set_float("u_material.shininess"_H, 32.0f);
}
