#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>
#include <glm/glm.hpp>

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

    load_platform();
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

    debug_add_line(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    debug_add_line(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void LevelScene::on_window_resized(bb::WindowResizedEvent& event) {
    cam.set_projection_matrix(event.width, event.height, LENS_FOV, LENS_NEAR, LENS_FAR);
}

void LevelScene::load_platform() {
    auto mesh {std::make_shared<bb::Mesh>(
        "data/models/platform.obj",
        "Walls",
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
    auto texture {cache_texture.load("platform"_H, "data/textures/old-bricks.jpeg", specification)};

    // This is a generic shader
    auto shader {std::make_shared<bb::Shader>(
        "data/shaders/simple_textured.vert",
        "data/shaders/simple_textured.frag"
    )};

    add_shader(shader);

    // And a generic material
    auto simple_material {std::make_shared<bb::Material>(shader)};
    simple_material->add_texture("u_material.ambient_diffuse"_H);
    simple_material->add_uniform(bb::Material::Uniform::Vec3, "u_material.specular"_H);
    simple_material->add_uniform(bb::Material::Uniform::Float, "u_material.shininess"_H);

    auto material_instance {cache_material_instance.load("platform"_H, simple_material)};
    material_instance->set_texture("u_material.ambient_diffuse"_H, texture, 0);
    material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.7f));
    material_instance->set_float("u_material.shininess"_H, 64.0f);
}
