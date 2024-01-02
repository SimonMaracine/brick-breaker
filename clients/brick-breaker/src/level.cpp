#include "level.hpp"

#include <cstddef>
#include <iterator>
#include <fstream>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <nlohmann/json.hpp>

#include "constants.hpp"

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
    connect_event<BallMissEvent, &LevelScene::on_ball_miss>(this);
    connect_event<BallBrickCollisionEvent, &LevelScene::on_ball_brick_collision>(this);

    load_shaders();
    load_platform();
    load_ball();
    load_paddle();
    load_brick();

    paddle = Paddle();
    balls.clear();
    balls[0u] = Ball(0u);
    bricks.clear();

    const auto level {load_level("data/levels/level1.json")};

    if (!level) {
        bb::log_message("Could not load level\n");
    } else {
        bricks = std::move(*level);
    }
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

#if SHOW_DEBUG_LINES
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

#if SHOW_DEBUG_LINES
        debug_add_line(ball.position - glm::vec3(ball.radius, 0.0f, 0.0f), ball.position + glm::vec3(ball.radius, 0.0f, 0.0f), GREEN);
        debug_add_line(ball.position - glm::vec3(0.0f, 0.0f, ball.radius), ball.position + glm::vec3(0.0f, 0.0f, ball.radius), GREEN);
#endif
    }

    for (const Brick& brick : bricks) {
        const auto material_id {resmanager::HashedStr64("brick" + std::to_string(static_cast<int>(brick.type) + 1))};

        bb::Renderable r_brick;
        r_brick.vertex_array = cache_vertex_array["brick"_H];
        r_brick.material = cache_material_instance[material_id];
        r_brick.position = brick.position;
        r_brick.scale = brick.get_scale();
        add_renderable(r_brick);

#if SHOW_DEBUG_LINES
        Box b;
        b.position = brick.position;
        b.width = brick.get_dimensions().x;
        b.height = brick.get_dimensions().y;
        b.depth = brick.get_dimensions().z;
        draw_bounding_box(b);
#endif
    }

#if SHOW_DEBUG_LINES
    debug_add_line(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    debug_add_line(glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, DEADLINE_Z), glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, DEADLINE_Z), GREEN);
    debug_add_line(glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, PLATFORM_EDGE_MIN_Z), glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, PLATFORM_EDGE_MIN_Z), GREEN);
    debug_add_line(glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, PLATFORM_EDGE_MIN_Z), glm::vec3(PLATFORM_EDGE_MIN_X, 0.0f, DEADLINE_Z), GREEN);
    debug_add_line(glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, PLATFORM_EDGE_MIN_Z), glm::vec3(PLATFORM_EDGE_MAX_X, 0.0f, DEADLINE_Z), GREEN);
#endif
}

void LevelScene::on_window_resized(const bb::WindowResizedEvent& event) {
    cam.set_projection_matrix(event.width, event.height, LENS_FOV, LENS_NEAR, LENS_FAR);
}

void LevelScene::on_key_pressed(const bb::KeyPressedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_LEFT:
            paddle.velocity = -11.0f;
            break;
        case bb::KeyCode::K_RIGHT:
            paddle.velocity = 11.0f;
            break;
        // case bb::KeyCode::K_i:
        //     balls[0].position.z -= 0.09f;
        //     break;
        // case bb::KeyCode::K_k:
        //     balls[0].position.z += 0.09f;
        //     break;
        // case bb::KeyCode::K_j:
        //     balls[0].position.x -= 0.09f;
        //     break;
        // case bb::KeyCode::K_l:
        //     balls[0].position.x += 0.09f;
        //     break;
        default:
            break;
    }
}

void LevelScene::on_key_released(const bb::KeyReleasedEvent& event) {
    switch (event.key) {
        case bb::KeyCode::K_LEFT:
        case bb::KeyCode::K_RIGHT:
            paddle.velocity = 0.0f;
            break;
        case bb::KeyCode::K_SPACE:
            shoot_balls();
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

    // TODO multiple textures and materials
    bb::TextureSpecification specification;
    cache_texture.load("brick1"_H, "data/textures/brick-texture1.png", specification);
    cache_texture.load("brick2"_H, "data/textures/brick-texture2.png", specification);
    cache_texture.load("brick3"_H, "data/textures/brick-texture3.png", specification);

    {
        auto material_instance {cache_material_instance.load("brick1"_H, cache_material["simple_textured"_H])};
        material_instance->set_texture("u_material.ambient_diffuse"_H, cache_texture["brick1"_H], 0);
        material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.4f));
        material_instance->set_float("u_material.shininess"_H, 32.0f);
    }

    {
        auto material_instance {cache_material_instance.load("brick2"_H, cache_material["simple_textured"_H])};
        material_instance->set_texture("u_material.ambient_diffuse"_H, cache_texture["brick2"_H], 0);
        material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.4f));
        material_instance->set_float("u_material.shininess"_H, 32.0f);
    }

    {
        auto material_instance {cache_material_instance.load("brick3"_H, cache_material["simple_textured"_H])};
        material_instance->set_texture("u_material.ambient_diffuse"_H, cache_texture["brick3"_H], 0);
        material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.4f));
        material_instance->set_float("u_material.shininess"_H, 32.0f);
    }
}

void LevelScene::update_collisions() {
    for (auto& [index, ball] : balls) {
        Sphere s;
        s.position = ball.position;
        s.radius = ball.radius;

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

    for (auto& [index, ball] : balls) {
        for (std::size_t i {0}; i < bricks.size(); i++) {
            const Brick& brick {bricks[i]};

            Sphere s;
            s.position = ball.position;
            s.radius = ball.radius;

            Box b;
            b.position = brick.position;
            b.width = brick.get_dimensions().x;
            b.height = brick.get_dimensions().y;
            b.depth = brick.get_dimensions().z;

            if (collision_sphere_box(s, b)) {
                bb::log_message("Collided!\n");

                // This i index may be valid only for a short time
                enqueue_event<BallBrickCollisionEvent>(index, i);
            }
        }
    }
}

void LevelScene::update_paddle(Paddle& paddle) {
    paddle.position += paddle.velocity * get_delta();

    if (paddle.position < PLATFORM_EDGE_MIN_X) {
        paddle.position = PLATFORM_EDGE_MIN_X;
    }

    if (paddle.position > PLATFORM_EDGE_MAX_X) {
        paddle.position = PLATFORM_EDGE_MAX_X;
    }
}

void LevelScene::update_ball(Ball& ball) {
    ball.position += ball.velocity * get_delta();

    if (ball.attached_to_paddle) {
        // Override position
        ball.position.x = paddle.position;
        ball.position.z = paddle.get_position().z - 1.0f;
    }

    const auto perpendicular_velocity {glm::rotate(glm::normalize(ball.velocity), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))};

    glm::mat4 trans {glm::mat4(1.0f)};
    trans = glm::translate(trans, ball.position);
    trans *= glm::toMat4(ball.rotation);
    trans = glm::rotate(trans, glm::length(ball.velocity) * 0.1f, perpendicular_velocity);  // TODO
    trans = glm::scale(trans, glm::vec3(ball.radius));  // Default ball size should be 1 meter in radius, so radius is scale

    ball.transformation = trans;

    const glm::quat rot {glm::angleAxis(glm::length(ball.velocity) * 0.01f, perpendicular_velocity)};
    ball.rotation = rot * ball.rotation;

    if (ball.position.x > PLATFORM_EDGE_MAX_X - ball.radius) {
        ball.position.x = PLATFORM_EDGE_MAX_X - ball.radius;
        ball.velocity.x *= -1.0f;
    }

    if (ball.position.x < PLATFORM_EDGE_MIN_X + ball.radius) {
        ball.position.x = PLATFORM_EDGE_MIN_X + ball.radius;
        ball.velocity.x *= -1.0f;
    }

    if (ball.position.z < PLATFORM_EDGE_MIN_Z + ball.radius) {
        ball.position.z = PLATFORM_EDGE_MIN_Z + ball.radius;
        ball.velocity.z *= -1.0f;
    }

    if (ball.position.z > DEADLINE_Z) {
        enqueue_event<BallMissEvent>(ball.index);
    }
}

void LevelScene::shoot_balls() {
    for (auto& [_, ball] : balls) {
        if (ball.attached_to_paddle) {
            ball.velocity = glm::vec3(glm::linearRand(-4.0f, 4.0f), 0.0f, -SHOOT_VELOCITY_Z);
            ball.attached_to_paddle = false;
        }
    }
}

std::optional<std::vector<Brick>> LevelScene::load_level(const std::string& file_path) {
    std::ifstream file {file_path};

    if (!file.is_open()) {
        return std::nullopt;
    }

    std::vector<Brick> result;

    const nlohmann::json root = nlohmann::json::parse(file);

    try {
        const nlohmann::json& j_bricks {root["bricks"].get<nlohmann::json>()};

        for (const nlohmann::json& j_brick : j_bricks) {
            const nlohmann::json& position {j_brick["position"].get<nlohmann::json>()};
            const int type {j_brick["type"].get<int>()};

            const int x {position[0].get<int>()};
            const int z {position[1].get<int>()};

            if (x < BRICKS_GRID_MIN_X || x > BRICKS_GRID_MAX_X || z < BRICKS_GRID_MIN_Z || z > BRICKS_GRID_MAX_Z) {
                return std::nullopt;
            }

            // TODO not best error detection
            if (type < 0 || type > 2) {
                return std::nullopt;
            }

            result.push_back(Brick(x, z, static_cast<BrickType>(type)));
        }
    } catch (const nlohmann::json::exception&) {
        return std::nullopt;
    }

    return std::make_optional(result);
}

void LevelScene::on_ball_paddle_collision(const BallPaddleCollisionEvent& event) {
    Ball& ball {balls[event.ball_index]};  // TODO can fail

    ball.position.z = paddle.get_position().z - paddle.get_dimensions().z - ball.radius;
    ball.velocity.z *= -1.0f;
}

void LevelScene::on_ball_miss(const BallMissEvent& event) {
    Ball& ball {balls[event.ball_index]};  // TODO can fail

    balls.erase(ball.index);

    if (balls.empty()) {
        bb::log_message("Game over!\n");  // TODO
    }
}

void LevelScene::on_ball_brick_collision(const BallBrickCollisionEvent& event) {
    Ball& ball {balls[event.ball_index]};  // TODO can fail

    bricks.erase(std::next(bricks.cbegin(), event.brick_index));

    ball.velocity.z *= -1.0f;  // TODO conditional  // FIXME check side

    if (bricks.empty()) {
        bb::log_message("Congratulations!\n");  // TODO
    }
}

void LevelScene::draw_bounding_box(const Box& box) {
    static constexpr auto color {GREEN};
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
