#include <string>
#include <cstdint>

#include <glad/glad.h>
#include <SDL.h>

#include "engine/window.hpp"
#include "engine/panic.hpp"
#include "engine/logging.hpp"
#include "engine/application.hpp"

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

        std::uint32_t flags {SDL_WINDOW_OPENGL};

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
                    application->events.enqueue(EventType::WindowClosed);

                    break;
                case SDL_KEYDOWN:
                    // event.key.keysym.sym

                    break;
                case SDL_KEYUP:
                    // event.key.keysym.sym

                    break;
                case SDL_MOUSEMOTION:

                    break;
                case SDL_MOUSEBUTTONDOWN:

                    break;
                case SDL_MOUSEBUTTONUP:

                    break;
                case SDL_MOUSEWHEEL:

                    break;
            }
        }
    }

    void Window::refresh() {
        SDL_GL_SwapWindow(window);
    }
}
