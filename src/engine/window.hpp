#pragma once

#include <string>

struct SDL_Window;

namespace bb {
    class Application;

    class Window {
    public:
        Window(int width, int height, const std::string& title, bool fullscreen, Application* application);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        void poll_events();
        void refresh();
    private:
        SDL_Window* window {nullptr};
        void* context {nullptr};
        Application* application {nullptr};
    };
}
