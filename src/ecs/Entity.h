#ifndef ZWEI_ENTITY_H
#define ZWEI_ENTITY_H

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

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

    void render() {
        if (!isActive) return;

        for (const auto &component: components) {
            component.second->render();
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

private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    bool isActive;
};

#endif
