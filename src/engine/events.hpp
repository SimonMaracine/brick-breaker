#pragma once

#include <utility>

#include <entt/signal/dispatcher.hpp>

#include "engine/input.hpp"

namespace bb {
    struct WindowClosedEvent {};

    struct KeyPressedEvent {
        KeyCode key;
    };

    struct KeyReleasedEvent {
        KeyCode key;
    };

    struct MouseMovedEvent {
        unsigned int buttons;
        int x, y;
        int xrel, yrel;
    };

    struct MouseButtonPressedEvent {
        unsigned char button;
        int x, y;
    };

    struct MouseButtonReleasedEvent {
        unsigned char button;
        int x, y;
    };

    struct MouseWheelScrolledEvent {
        int scroll;
    };

    class EventSystem {
    public:
        template<typename E, typename... Args>
        void enqueue(Args&&... args) {
            dispatcher.template enqueue<E>(std::forward<Args>(args)...);
        }

        template<typename E, auto F, typename T>
        void connect(T&& instance) {
            dispatcher.template sink<E>().template connect<F, T>(std::forward<T>(instance));
        }

        template<typename T>
        void disconnect(T&& instance) {
            dispatcher.disconnect(std::forward<T>(instance));
        }

        void update() {
            dispatcher.update();
        }
    private:
        entt::dispatcher dispatcher;
    };
}
