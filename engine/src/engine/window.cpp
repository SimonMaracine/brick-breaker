#include <string>

#ifdef BB_CHRONO_TIMER
    #include <chrono>
#endif

#include <glad/glad.h>
#include <SDL.h>

#include "engine/window.hpp"
#include "engine/panic.hpp"
#include "engine/logging.hpp"
#include "engine/application.hpp"
#include "engine/input.hpp"

namespace bb {
    Window::Window(const WindowProperties& properties, Application* application)
        : width(properties.width), height(properties.height), application(application) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
            log_message("Could not initialize SDL\n");
            throw InitializationError;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        unsigned int flags {SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE};

        if (properties.fullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        }

        window = SDL_CreateWindow(
            properties.title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            flags
        );

        if (window == nullptr) {
            log_message("Could not create window\n");
            throw InitializationError;
        }

        context = SDL_GL_CreateContext(window);

        if (!gladLoadGL()) {
            log_message("Could not initialize glad\n");
            throw InitializationError;
        }

        if (SDL_GL_SetSwapInterval(1) < 0) {
            throw InitializationError;
        }

        SDL_SetWindowMinimumSize(window, properties.min_width, properties.min_height);
    }

    Window::~Window() {
        SDL_GL_DeleteContext(static_cast<SDL_GLContext>(context));
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Window::set_vsync(bool enabled) {
        if (SDL_GL_SetSwapInterval(static_cast<int>(enabled)) < 0) {
            throw OtherError;
        }
    }

    void Window::capture_mouse(bool enabled) {
        if (SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) < 0) {
            throw OtherError;
        }
    }

    void Window::poll_events() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    application->events.enqueue<WindowClosedEvent>();

                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            application->events.enqueue<WindowResizedEvent>(event.window.data1, event.window.data2);

                            width = event.window.data1;
                            height = event.window.data2;

                            break;
                        default:
                            break;
                    }

                    break;
                case SDL_KEYDOWN:
                    application->events.enqueue<KeyPressedEvent>(
                        static_cast<KeyCode>(event.key.keysym.sym),
                        static_cast<bool>(event.key.repeat)
                    );

                    break;
                case SDL_KEYUP:
                    application->events.enqueue<KeyReleasedEvent>(static_cast<KeyCode>(event.key.keysym.sym));

                    break;
                case SDL_MOUSEMOTION:
                    application->events.enqueue<MouseMovedEvent>(
                        event.motion.state,
                        event.motion.x,
                        event.motion.y,
                        event.motion.xrel,
                        event.motion.yrel
                    );

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    application->events.enqueue<MouseButtonPressedEvent>(
                        event.button.button,
                        event.button.x,
                        event.button.y
                    );

                    break;
                case SDL_MOUSEBUTTONUP:
                    application->events.enqueue<MouseButtonReleasedEvent>(
                        event.button.button,
                        event.button.x,
                        event.button.y
                    );

                    break;
                case SDL_MOUSEWHEEL:
                    application->events.enqueue<MouseWheelScrolledEvent>(event.wheel.y);

                    break;
            }
        }
    }

    void Window::refresh() const {
        SDL_GL_SwapWindow(window);
    }

    double Window::get_time() {
#ifdef BB_CHRONO_TIMER
        return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
#else
        return static_cast<double>(SDL_GetTicks64()) / 1000.0;
#endif
    }
}
