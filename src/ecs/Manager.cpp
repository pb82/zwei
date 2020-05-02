#include "Manager.h"

std::shared_ptr<Entity> Manager::addEntity() {
    auto entity = std::make_shared<Entity>();
    entities.push_back(entity);
    return entity;
}

void Manager::update(float dt) {
    for (auto entity : entities) {
        entity->update(dt);
    }
}

void Manager::render(LayerType layer) {
    for (auto entity : entities) {
        entity->render(layer);
    }
}