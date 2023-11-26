#pragma once

#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>

namespace bb {
    enum class EventType {
        WindowClosed,
        KeyPressed,
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrolled
    };

    struct WindowClosedEvent {};

    struct KeyPressedEvent {

    };

    struct KeyReleasedEvent {

    };

    struct MouseMovedEvent {

    };

    struct MouseButtonPressedEvent {

    };

    struct MouseButtonReleasedEvent {

    };

    struct MouseScrolledEvent {

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
            MouseScrolledEvent ms;
        };
    };

    class EventSystem {
    public:
        template<typename... Args>
        void enqueue(EventType type, Args&&... args) {
            Event event;
            event.type = type;

            switch (type) {
                case EventType::WindowClosed: {
                    WindowClosedEvent event_type {std::forward<Args>(args)...};
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
                case EventType::MouseScrolled: {
                    MouseScrolledEvent event_type {std::forward<Args>(args)...};
                    event.ms = event_type;

                    break;
                }
            }

            events.push_back(event);
        }

        void connect(EventType type, const std::function<void(const Event&)>& callback);
        void disconnect(EventType type);
        void disconnect();
        void update();
    private:
        std::vector<Event> events;
        std::unordered_map<EventType, std::function<void(const Event&)>> callbacks;
    };
}
