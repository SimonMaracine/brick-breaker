#include <string>

#include <glad/glad.h>
#include <SDL.h>

#include "engine/window.hpp"
#include "engine/panic.hpp"
#include "engine/logging.hpp"
#include "engine/application.hpp"
#include "engine/input.hpp"

namespace bb {
    Window::Window(int width, int height, const std::string& title, bool fullscreen, Application* application)
        : application(application) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log_message("Could not initialize SDL\n");
            throw InitializationError;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        unsigned int flags {SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE};

        if (fullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        }

        window = SDL_CreateWindow(
            title.c_str(),
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
    }

    Window::~Window() {
        SDL_GL_DeleteContext(static_cast<SDL_GLContext>(context));
        SDL_DestroyWindow(window);
        SDL_Quit();
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

                            break;
                        default:
                            break;
                    }

                    break;
                case SDL_KEYDOWN:
                    application->events.enqueue<KeyPressedEvent>(static_cast<KeyCode>(event.key.keysym.sym));

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

    void Window::refresh() {
        SDL_GL_SwapWindow(window);
    }
}
