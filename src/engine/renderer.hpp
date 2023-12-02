#pragma once

namespace bb {
    class Application;

    class Renderer {
    public:
        Renderer(int width, int height);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;
    private:
        void render(int width, int height);

        friend class Application;
    };
}
