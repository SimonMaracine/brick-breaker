#pragma once

#include <memory>

#include "engine/events.hpp"
#include "engine/window.hpp"
#include "engine/application_properties.hpp"

namespace bb {
    class Application {
    public:
        Application(const ApplicationProperties& properties);
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        void run();
    private:
        EventSystem events;
        std::unique_ptr<Window> window;

        bool running {true};

        friend class Window;
    };
}
