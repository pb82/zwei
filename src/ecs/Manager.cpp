#include "Manager.h"

#include "Collider.h"

std::shared_ptr<Entity> Manager::addEntity(LayerType layer) {
    auto entity = std::make_shared<Entity>();
    if (entities.find(layer) == entities.end()) {
        entities.emplace(layer, std::vector<std::shared_ptr<Entity>>());
    }
    entities.at(layer).push_back(entity);
    return entity;
}

void Manager::update(float dt) {
    for (auto &layer : entities) {
        for (auto &entity : layer.second) {
            entity->update(dt);
        }
    }
}

void Manager::render(LayerType layer) {
    if (entities.find(layer) == entities.end()) {
        return;
    }

    for (auto &entity : entities.at(layer)) {
        entity->render();
    }
}

void Manager::key(GameKeyEvent &key) {
    for (auto &entity : entities.at(OBJECTS)) {
        entity->key(key);
    }
}

void Manager::getColliders(std::vector<std::shared_ptr<Collider> > &target) {
    for (auto &entity : entities.at(WALLS)) {
        if (entity->hasComponent<Collider>()) {
            auto collider = entity->getComponent<Collider>();

            // Reset the checked flag every time, otherwise every entity
            // would only get checked once
            collider->checked = false;
            target.push_back(collider);
        }
    }
    for (auto &entity : entities.at(OBJECTS)) {
        if (entity->hasComponent<Collider>()) {
            auto collider = entity->getComponent<Collider>();
            collider->checked = false;
            target.push_back(collider);
        }
    }
}