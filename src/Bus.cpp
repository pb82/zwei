#include "Bus.h"

void Bus::subscribe(EventType type, std::function<void(const Event &)> cb) {
    subscribers[static_cast<int>(type)].push_back(std::move(cb));
}

void Bus::publish(const Event &event) {
    auto it = subscribers.find(static_cast<int>(event.type));
    if (it == subscribers.end()) return;
    for (auto &cb : it->second) {
        cb(event);
    }
}
