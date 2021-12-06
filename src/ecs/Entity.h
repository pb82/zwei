#ifndef ZWEI_ENTITY_H
#define ZWEI_ENTITY_H

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <queue>

#include "Component.h"

class Entity {
public:
    Entity() : isActive(true) {}

    template<typename T, typename... TArgs>
    std::shared_ptr<Component> addComponent(TArgs... args) {
        auto component = new T(*this, std::forward<TArgs>(args)...);
        auto type = std::type_index(typeid(T));
        components.emplace(type, component);
        return components[type];
    }

    template<typename T>
    bool hasComponent() const {
        auto type = std::type_index(typeid(T));
        return components.find(type) != components.end();
    }

    template<typename T>
    void removeComponent() {
        auto type = std::type_index(typeid(T));
        pendingRemoval.push(type);
    }

    template<typename T>
    std::shared_ptr<T> getComponent() {
        auto type = std::type_index(typeid(T));
        auto component = components[type];
        return std::dynamic_pointer_cast<T>(component);
    }

    void update(float dt) {
        if (!isActive) return;

        for (const auto &component: components) {
            component.second->update(dt);
        }
    }

    void render(uint8_t hints) {
        if (!isActive) return;

        for (const auto &component: components) {
            component.second->render(hints);
        }
    }

    void key(GameKeyEvent &key) {
        if (!isActive) return;

        for (const auto &component: components) {
            component.second->key(key);
        }
    }

    void disable() {
        this->isActive = false;
    }

    void collect() {
        if (!pendingRemoval.empty()) {
            components.erase(pendingRemoval.front());
        }
    }

    bool disabled() {
        return !this->isActive;
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;

    std::queue<std::type_index> pendingRemoval;

    bool isActive;
};

#endif
