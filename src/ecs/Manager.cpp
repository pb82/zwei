#include "Manager.h"

#include "Collider.h"
#include "Group.h"

Manager::Manager() {
    entities.emplace(LayerType::FOREGROUND, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::ITEMS, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::OBJECTS, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::BACKGROUND, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::FLOOR, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::UI, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::WALLS, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::ROOF, std::vector<std::shared_ptr<Entity>>());
}

void Manager::setRenderHint(uint8_t hint) {
    renderHints |= hint;
}

void Manager::clearRenderHint(uint8_t hint) {
    renderHints &= ~(hint);
}

std::shared_ptr<Entity> Manager::addEntity(LayerType layer) {
    auto entity = std::make_shared<Entity>();
    entities.at(layer).push_back(entity);
    return entity;
}

void Manager::enqueue(std::shared_ptr<Entity> entity, LayerType layer) {
    QueueEntity e;
    e.entity = entity;
    e.layer = layer;
    pendingEntities.push(e);
}

void Manager::collect() {
    // New entities?
    if (!pendingEntities.empty()) {
        auto entity = pendingEntities.front();
        if (entity.entity->hasComponent<Group>()) {
            auto group = entity.entity->getComponent<Group>();
            for (auto member : group->all()) {
                entities.at(entity.layer).push_back(member);
            }
            pendingEntities.pop();
        } else {
            entities.at(entity.layer).push_back(entity.entity);
            pendingEntities.pop();
        }
    }

    // Garbage collect
    for (auto &layer : entities) {
        auto &v = layer.second;
        v.erase(std::remove_if(v.begin(), v.end(), [](std::shared_ptr<Entity> e) {
            return e->disabled();
        }), v.end());
    }
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

    if (layer == ROOF && (renderHints & HINT_HIDE_ROOF_LAYER) == HINT_HIDE_ROOF_LAYER) {
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

void Manager::uiInput(GameKeyEvent &key) {
    for (auto &entity : entities.at(UI)) {
        entity->key(key);
    }
}

void Manager::getColliders(std::vector<std::shared_ptr<Collider> > &target) {
    for (auto &entity : entities.at(WALLS)) {
        if (entity->hasComponent<Collider>()) {
            auto collider = entity->getComponent<Collider>();
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
    for (auto &entity : entities.at(ITEMS)) {
        if (entity->hasComponent<Collider>()) {
            auto collider = entity->getComponent<Collider>();
            collider->checked = false;
            target.push_back(collider);
        }
    }
}

bool Manager::hasEntities(Position p, LayerType layer) {
    for (auto &entity : entities.at(layer)) {
        if (entity->hasComponent<Transform>()) {
            auto t = entity->getComponent<Transform>();
            if (p == t->p) {
                return true;
            }
        }
    }
    return false;
}