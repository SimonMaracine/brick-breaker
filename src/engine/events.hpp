#pragma once

#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>

#include "engine/input.hpp"

namespace bb {
    enum class EventType {
        WindowClosed,
        KeyPressed,
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseWheelScrolled
    };

    struct WindowClosedEvent {};

    struct KeyPressedEvent {
        KeyCode key;
    };

    struct KeyReleasedEvent {
        KeyCode key;
    };

    struct MouseMovedEvent {
        unsigned char buttons;
        unsigned short x, y;
        unsigned short xrel, yrel;
    };

    struct MouseButtonPressedEvent {
        unsigned char button;
        unsigned short x, y;
    };

    struct MouseButtonReleasedEvent {
        unsigned char button;
        unsigned short x, y;
    };

    struct MouseWheelScrolledEvent {
        int scroll;
    };

    struct Event {
        EventType type;

        union {
            WindowClosedEvent wc;
            KeyPressedEvent kp;
            KeyReleasedEvent kr;
            MouseMovedEvent mm;
            MouseButtonPressedEvent mbp;
            MouseButtonReleasedEvent mbr;
            MouseWheelScrolledEvent mws;
        };
    };

    class EventSystem {
    public:
        using EventCallback = std::function<void(const Event&)>;

        template<typename... Args>
        void enqueue(EventType type, Args&&... args) {
            Event event;
            event.type = type;

            switch (type) {
                case EventType::WindowClosed: {
                    WindowClosedEvent event_type;
                    event.wc = event_type;

                    break;
                }
                case EventType::KeyPressed: {
                    KeyPressedEvent event_type {std::forward<Args>(args)...};
                    event.kp = event_type;

                    break;
                }
                case EventType::KeyReleased: {
                    KeyReleasedEvent event_type {std::forward<Args>(args)...};
                    event.kr = event_type;

                    break;
                }
                case EventType::MouseMoved: {
                    MouseMovedEvent event_type {std::forward<Args>(args)...};
                    event.mm = event_type;

                    break;
                }
                case EventType::MouseButtonPressed: {
                    MouseButtonPressedEvent event_type {std::forward<Args>(args)...};
                    event.mbp = event_type;

                    break;
                }
                case EventType::MouseButtonReleased: {
                    MouseButtonReleasedEvent event_type {std::forward<Args>(args)...};
                    event.mbr = event_type;

                    break;
                }
                case EventType::MouseWheelScrolled: {
                    MouseWheelScrolledEvent event_type {std::forward<Args>(args)...};
                    event.mws = event_type;

                    break;
                }
            }

            events.push_back(event);
        }

        void connect(EventType type, const EventCallback& callback);
        void disconnect(EventType type);
        void disconnect();
        void update();
    private:
        std::vector<Event> events;
        std::unordered_map<EventType, EventCallback> callbacks;
    };
}
