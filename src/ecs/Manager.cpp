#include "Manager.h"

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
        entity->render(layer);
    }
}