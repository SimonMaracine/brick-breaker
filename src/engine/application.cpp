#include <memory>

#include <SDL.h>

#include "engine/events.hpp"
#include "engine/window.hpp"
#include "engine/application_properties.hpp"
#include "engine/application.hpp"
#include "logging.hpp"

namespace bb {
    Application::Application(const ApplicationProperties& properties) {
        window = std::make_unique<Window>(
            properties.width,
            properties.height,
            properties.title,
            properties.fullscreen,
            this
        );

        events.connect(EventType::WindowClosed, [this](const Event&) {
            running = false;
        });

        log_message("Initialized application\n");
    }

    Application::~Application() {
        log_message("Destroyed application\n");
    }

    void Application::run() {
        while (running) {
            window->poll_events();
            events.update();

            // TODO rendering

            window->refresh();
        }
    }
}
