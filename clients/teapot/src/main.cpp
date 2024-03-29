#include <memory>

#include <engine/engine.hpp>
#include <resmanager/resmanager.hpp>

#include "my_camera_controller.hpp"

using namespace resmanager::literals;

struct MainScene : public bb::Scene {
    MainScene()
        : bb::Scene("main") {}

    virtual void on_enter() override {
        bb::log_message("Entered main scene\n");

        connect_event<bb::KeyPressedEvent, &MainScene::on_key_pressed>(this);
        connect_event<bb::KeyReleasedEvent, &MainScene::on_key_released>(this);
        connect_event<bb::MouseMovedEvent, &MainScene::on_mouse_moved>(this);
        connect_event<bb::MouseButtonPressedEvent, &MainScene::on_mouse_button_pressed>(this);
        connect_event<bb::MouseButtonReleasedEvent, &MainScene::on_mouse_button_released>(this);
        connect_event<bb::MouseWheelScrolledEvent, &MainScene::on_mouse_wheel_scrolled>(this);
        connect_event<bb::WindowResizedEvent, &MainScene::on_window_resized>(this);

        auto mesh {std::make_shared<bb::Mesh>(
            "data/models/teapot.obj",
            bb::Mesh::DEFAULT_OBJECT,
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

        teapot_vertex_array = std::make_shared<bb::VertexArray>();
        teapot_vertex_array->configure([&](bb::VertexArray* va) {
            bb::VertexBufferLayout layout;
            layout.add(0, bb::VertexBufferLayout::Float, 3);
            layout.add(1, bb::VertexBufferLayout::Float, 3);

            va->add_vertex_buffer(vertex_buffer, layout);
            va->add_index_buffer(index_buffer);
        });

        auto shader {std::make_shared<bb::Shader>(
            "data/shaders/simple.vert",
            "data/shaders/simple.frag"
        )};

        add_shader(shader);

        simple_material = std::make_shared<bb::Material>(shader);
        simple_material->add_uniform(bb::Material::Uniform::Vec3, "u_material.ambient_diffuse"_H);
        simple_material->add_uniform(bb::Material::Uniform::Vec3, "u_material.specular"_H);
        simple_material->add_uniform(bb::Material::Uniform::Float, "u_material.shininess"_H);

        teapot_material_instance = std::make_shared<bb::MaterialInstance>(simple_material);
        teapot_material_instance->set_vec3("u_material.ambient_diffuse"_H, glm::vec3(0.7f));
        teapot_material_instance->set_vec3("u_material.specular"_H, glm::vec3(0.7f));
        teapot_material_instance->set_float("u_material.shininess"_H, 64.0f);
        teapot_material_instance->flags |= bb::Material::DisableBackFaceCulling;

        teapot_material_instance2 = std::make_shared<bb::MaterialInstance>(simple_material);
        teapot_material_instance2->set_vec3("u_material.ambient_diffuse"_H, glm::vec3(0.25f, 0.2f, 0.7f));
        teapot_material_instance2->set_vec3("u_material.specular"_H, glm::vec3(0.9f));
        teapot_material_instance2->set_float("u_material.shininess"_H, 128.0f);
        teapot_material_instance2->flags |= bb::Material::DisableBackFaceCulling;

        cam_controller = MyCameraController(
            &cam,
            get_width(),
            get_height(),
            LENS_FOV,
            LENS_NEAR,
            LENS_FAR,
            glm::vec3(0.0f),
            8.0f,
            47.0f
        );

        connect_event<bb::KeyPressedEvent, &MyCameraController::on_key_pressed>(cam_controller);

        directional_light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        directional_light.ambient_color = glm::vec3(0.1f);
        directional_light.diffuse_color = glm::vec3(1.0f);
        directional_light.specular_color = glm::vec3(1.0f);

        bb::OpenGl::clear_color(0.4f, 0.15f, 0.25f);
    }

    virtual void on_exit() override {
        bb::log_message("Exited main scene\n");
    }

    virtual void on_update() override {
        cam_controller.update_controls(get_delta());
        cam_controller.update_camera(get_delta());

        capture(cam, cam_controller.get_position());
        add_light(directional_light);

        teapot_rotation += 1.0f * get_delta();
        teapot_position_y += static_cast<float>(glm::sin(bb::Window::get_time()) / 2.0) * get_delta();

        bb::Renderable teapot;
        teapot.vertex_array = teapot_vertex_array;
        teapot.material = teapot_material_instance;
        teapot.rotation = glm::vec3(0.0f, teapot_rotation, 0.0f);
        teapot.scale = 0.8f;

        add_renderable(teapot);

        teapot.material = teapot_material_instance2;
        teapot.position = glm::vec3(-3.0f, teapot_position_y, -1.0f);
        teapot.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
        teapot.scale = 0.4f;

        add_renderable(teapot);
    }

    void on_key_pressed(const bb::KeyPressedEvent& event) {
        bb::log_message("KeyPressed: %d\n", static_cast<int>(event.key));

        if (event.key == bb::KeyCode::K_f) {
            bb::log_message("%f - %f\n", get_delta(), get_fps());
        }
    }

    void on_key_released(const bb::KeyReleasedEvent& event) {
        bb::log_message("KeyReleased: %d\n", static_cast<int>(event.key));

        static bool enabled {true};

        if (event.key == bb::KeyCode::K_v) {
            enabled = !enabled;
            set_vsync(enabled);
        }
    }

    void on_mouse_moved(const bb::MouseMovedEvent& event) {
        bb::log_message("MouseMoved: %dx%d\n", event.x, event.y);
    }

    void on_mouse_button_pressed(const bb::MouseButtonPressedEvent& event) {
        bb::log_message("MouseButtonPressed: %d\n", static_cast<int>(event.button));
    }

    void on_mouse_button_released(const bb::MouseButtonReleasedEvent& event) {
        bb::log_message("MouseButtonReleased: %d\n", static_cast<int>(event.button));
    }

    void on_mouse_wheel_scrolled(const bb::MouseWheelScrolledEvent& event) {
        bb::log_message("MouseWheelScrolled: %d\n", event.scroll);
    }

    void on_window_resized(const bb::WindowResizedEvent& event) {
        bb::log_message("WindowResized: %dx%d\n", event.width, event.height);

        cam.set_projection_matrix(event.width, event.height, LENS_FOV, LENS_NEAR, LENS_FAR);
    }

    bb::Camera cam;
    MyCameraController cam_controller;

    bb::DirectionalLight directional_light;

    std::shared_ptr<bb::Material> simple_material;
    std::shared_ptr<bb::VertexArray> teapot_vertex_array;
    std::shared_ptr<bb::MaterialInstance> teapot_material_instance;
    std::shared_ptr<bb::MaterialInstance> teapot_material_instance2;

    float teapot_rotation {0.0f};
    float teapot_position_y {0.0f};
};

int main() {
    bb::ApplicationProperties properties;

    try {
        bb::Application application {properties};
        application.add_scene<MainScene>();
        application.run("main");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
