#include <sstream>
#include "Stats.h"

#include "Entity.h"
#include "Manager.h"
#include "Sprite.h"
#include "Animation.h"
#include "Transform.h"
#include "SelfDestruct.h"

Stats::Stats(Entity &parent, bool render) : Component(parent), inventory(parent) {}

bool Stats::serialize(JSON::Value &to) {
    JSON::Value i;
    JSON::Value c;
    this->inventory.serialize(i);
    this->character.serialize(c);
    to["inventory"] = i;
    to["character"] = c;
    return true;
}

void Stats::update(float dt) {
    inventory.update(dt);
    auto hp = this->character.getHitpoints();
    if (std::get<0>(hp) <= 0) {
        // Removed entity is no longer blocking the way
        auto transform = this->parent.getComponent<Transform>();
        RT_Context.getTopology().unregisterMobile(&transform->p);

        this->parent.disable();

        // Replace the enemy with an explosion
        auto entity = std::make_shared<Entity>();
        entity->addComponent<Sprite>(SPRITES);
        entity->addComponent<Animation>(100, false);

        auto explosion = entity->getComponent<Animation>();
        explosion->addAnimationFrame(128);
        explosion->addAnimationFrame(129);
        explosion->addAnimationFrame(130);
        explosion->addAnimationFrame(131);

        entity->addComponent<SelfDestruct>(TIMER, 400);

        auto parentTransform = this->parent.getComponent<Transform>();
        entity->addComponent<Transform>(parentTransform->p.x, parentTransform->p.y);

        Manager::instance().enqueue(entity, FOREGROUND);
    }
}