#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>

#include "engine/events.hpp"

namespace bb {
    void EventSystem::connect(EventType type, const std::function<void(const Event&)>& callback) {
        callbacks[type] = callback;
    }

    void EventSystem::disconnect(EventType type) {
        callbacks.erase(type);
    }

    void EventSystem::disconnect() {
        callbacks.clear();
    }

    void EventSystem::update() {
        for (const Event& event : events) {
            if (callbacks.count(event.type) == 1) {
                callbacks.at(event.type)(event);
            }
        }

        events.clear();
    }
}
