#ifndef ZWEI_ENTITY_H
#define ZWEI_ENTITY_H

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

#include "Component.h"

class Entity {
public:
    template<typename T, typename... TArgs>
    std::shared_ptr<Component> addComponent(TArgs... args) {
        auto component = new T(*this, std::forward<TArgs>(args)...);
        auto type = std::type_index(typeid(T));
        components.emplace(type, component);
        return components[type];
    }

    template<typename T>
    bool hasComponent() {
        auto type = std::type_index(typeid(T));
        return components.find(type) != components.end();
    }

    void update() {
        for (const auto &component: components) {
            component.second->update();
        }
    }

    void render() {
        for (const auto &component: components) {
            component.second->render();
        }
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
};

#endif
