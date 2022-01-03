#include "Manager.h"

#include "Collider.h"
#include "Group.h"
#include "Interactible.h"
#include "Timer.h"
#include "Id.h"
#include "Collectable.h"
#include "Ai.h"
#include "Hostile.h"
#include "Friend.h"

Manager::Manager() {
    init();
}

void Manager::init() {
    entities.emplace(LayerType::FOREGROUND, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::ITEMS, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::OBJECTS, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::BACKGROUND, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::FLOOR, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::UI, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::WALLS, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::ROOF, std::vector<std::shared_ptr<Entity>>());
    entities.emplace(LayerType::SKY, std::vector<std::shared_ptr<Entity>>());

    // Globally accessible timer component
    timers = std::make_shared<Entity>();
    timers->addComponent<Timer>();
    enqueue(timers, FOREGROUND);
}

void Manager::resetAll() {
    entities.clear();
    while (!pendingEntities.empty()) pendingEntities.pop();
    timers.reset();
    renderHints = 0;
    init();
}

void Manager::addTimer(int tile, float duration) {
    auto t = timers->getComponent<Timer>();
    t->addTimer(tile, duration);
}

void Manager::addTimer(int tile, float duration, float max) {
    auto t = timers->getComponent<Timer>();
    t->addTimer(tile, duration, max);
}

void Manager::addTimer(float duration, float max, const char *text) {
    auto t = timers->getComponent<Timer>();
    t->addTimer(duration, max, text);
}

void Manager::setRenderHint(uint8_t hint) {
    renderHints |= hint;
}

void Manager::setRenderHints(uint8_t hints) {
    renderHints = hints;
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
            for (auto member: group->all()) {
                entities.at(entity.layer).push_back(member);
            }
            pendingEntities.pop();
        } else {
            entities.at(entity.layer).push_back(entity.entity);
            pendingEntities.pop();
        }
    }

    // Garbage collect
    for (auto &layer: entities) {
        auto &v = layer.second;

        v.erase(std::remove_if(v.begin(), v.end(), [](std::shared_ptr<Entity> e) {
            return e->disabled();
        }), v.end());

        // Garbage collect entity components
        for (auto &e: v) {
            e->collect();
        }
    }
}

void Manager::update(float dt) {
    for (auto &layer: entities) {
        for (auto &entity: layer.second) {
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

    if (layer != OBJECTS) {
        for (auto &entity: entities.at(layer)) {
            entity->render(renderHints);
        }
    } else {
        // Perspective correction: sort entities to draw them from south to north
        std::sort(entities.at(OBJECTS).begin(), entities.at(OBJECTS).end(),
                  [](const std::shared_ptr<Entity> &a, const std::shared_ptr<Entity> &b) -> bool {
                      auto at = a->getComponent<Transform>();
                      auto bt = b->getComponent<Transform>();
                      return at->p.y <= bt->p.y;
                  });

        for (auto &entity: entities.at(OBJECTS)) {
            entity->render(renderHints);
        }
    }
}

void Manager::key(GameKeyEvent &key) {
    for (auto &entity: entities.at(OBJECTS)) {
        entity->key(key);
    }
}

void Manager::uiInput(GameKeyEvent &key) {
    for (auto &entity: entities.at(UI)) {
        entity->key(key);
    }
}

void Manager::getColliders(std::vector<std::shared_ptr<Collider> > &target) {
    for (auto &entity: entities.at(WALLS)) {
        if (entity->hasComponent<Collider>()) {
            auto collider = entity->getComponent<Collider>();
            if (collider->suspended) continue;
            collider->checked = false;
            target.push_back(collider);
        }
    }
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Collider>()) {
            auto collider = entity->getComponent<Collider>();
            if (collider->suspended) continue;
            collider->checked = false;
            target.push_back(collider);
        }
    }
    for (auto &entity: entities.at(ITEMS)) {
        if (entity->hasComponent<Collider>()) {
            auto collider = entity->getComponent<Collider>();
            if (collider->suspended) continue;
            collider->checked = false;
            target.push_back(collider);
        }
    }
}

void Manager::getInteractibles(std::vector<std::shared_ptr<Entity>> &target) {
    for (auto &entity: entities.at(WALLS)) {
        if (entity->hasComponent<Interactible>()) {
            target.push_back(entity);
        }
    }
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Interactible>()) {
            target.push_back(entity);
        }
    }
}

void Manager::getItems(std::vector<std::shared_ptr<Entity>> &target) {
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Collectable>()) {
            target.push_back(entity);
        }
    }
}

void Manager::getEnemies(std::vector<std::shared_ptr<Entity>> &target) {
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Hostile>()) {
            target.push_back(entity);
        }
    }
}

void Manager::getAllies(std::vector<std::shared_ptr<Entity>> &target) {
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Friend>()) {
            target.push_back(entity);
        }
    }
}

std::shared_ptr<Entity> Manager::getEnemy(uint8_t id) {
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Ai>() && entity->hasComponent<Id>()) {
            auto enemyId = entity->getComponent<Id>();
            if (enemyId->id == id) return entity;
        }
    }
    return nullptr;
}

std::shared_ptr<Interactible> Manager::getInteractible(int x, int y) {
    for (auto &entity: entities.at(WALLS)) {
        if (entity->hasComponent<Interactible>() && entity->hasComponent<Transform>()) {
            auto transform = entity->getComponent<Transform>();
            Position p;
            transform->p.nearestTile(p);
            if (p.x == x && p.y == y) {
                return entity->getComponent<Interactible>();
            }
        }
    }
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Interactible>() && entity->hasComponent<Transform>()) {
            auto transform = entity->getComponent<Transform>();
            Position p;
            transform->p.nearestTile(p);
            if (p.x == x && p.y == y) {
                return entity->getComponent<Interactible>();
            }
        }
    }
    return nullptr;
}

std::shared_ptr<Entity> Manager::getInteractible(uint8_t id) {
    for (auto &entity: entities.at(WALLS)) {
        if (entity->hasComponent<Interactible>() && entity->hasComponent<Id>()) {
            auto entityId = entity->getComponent<Id>();
            if (entityId->id == id) return entity;
        }
    }
    for (auto &entity: entities.at(OBJECTS)) {
        if (entity->hasComponent<Interactible>()) {
            auto entityId = entity->getComponent<Id>();
            if (entityId->id == id) return entity;
        }
    }
    return nullptr;
}


bool Manager::hasEntities(Position p, LayerType layer) {
    for (auto &entity: entities.at(layer)) {
        if (entity->hasComponent<Transform>()) {
            auto t = entity->getComponent<Transform>();
            if (p == t->p) {
                return true;
            }
        }
    }
    return false;
}

std::shared_ptr<Entity> Manager::getWall(Position &p) {
    for (auto &entity: entities.at(WALLS)) {
        if (entity->hasComponent<Transform>()) {
            auto t = entity->getComponent<Transform>();
            if (t->p == p) {
                return entity;
            }
        }
    }
    return nullptr;
}
