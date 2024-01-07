#include "main_scene.hpp"

void Main::on_enter() {
    specification.min_filter = bb::Filter::Nearest;
    specification.mag_filter = bb::Filter::Nearest;

    pixels = new std::uint32_t[get_width() * get_height()];

    // Casting directly might be bad
    texture = std::make_unique<bb::Texture>(get_width(), get_height(), reinterpret_cast<unsigned char*>(pixels), specification);

    const float vertices[] = {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f
    };

    std::shared_ptr<bb::VertexBuffer> buffer {std::make_shared<bb::VertexBuffer>(vertices, sizeof(vertices))};

    vertex_array = std::make_unique<bb::VertexArray>();
    vertex_array->configure([&](bb::VertexArray* va) {
        bb::VertexBufferLayout layout;
        layout.add(0, bb::VertexBufferLayout::Type::Float, 2);

        va->add_vertex_buffer(buffer, layout);
    });

    shader = std::make_unique<bb::Shader>(
        "data/shaders/screen_quad.vert",
        "data/shaders/screen_quad.frag"
    );

    connect_event<bb::WindowResizedEvent, &Main::on_window_resized>(this);
}

void Main::on_exit() {
    delete[] pixels;
}

void Main::on_update() {
    draw();

    bb::OpenGl::clear(bb::OpenGl::Buffers::C);

    texture->bind(0);
    texture->set_pixels(0, 0, get_width(), get_height(), pixels);

    shader->bind();
    vertex_array->bind();
    bb::OpenGl::draw_arrays(6);

    bb::VertexArray::unbind();
    bb::Shader::unbind();
    bb::Texture::unbind();
}

void Main::on_window_resized(const bb::WindowResizedEvent& event) {
    delete[] pixels;
    pixels = new std::uint32_t[event.width, event.height];

    texture = std::make_unique<bb::Texture>(event.width, event.height, reinterpret_cast<unsigned char*>(pixels), specification);

    bb::OpenGl::viewport(event.width, event.height);
}

void Main::draw() {
    for (int x {0}; x < get_width(); x++) {
        for (int y {0}; y < get_height(); y++) {
            pixels[x + y * get_width()] = 1;
        }
    }
}
