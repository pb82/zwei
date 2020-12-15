#include "Stats.h"

#include "Entity.h"
#include "Manager.h"
#include "Sprite.h"
#include "Animation.h"
#include "Transform.h"
#include "SelfDestruct.h"

Stats::Stats(Entity &parent) : Component(parent), life(0), maxLife(0) {}

void Stats::equipWeapon(std::shared_ptr<Weapon> w) {
    this->weapon = w;
}

void Stats::dropWeapon() {
    this->weapon = nullptr;
}

bool Stats::hasWeapon() {
    if (this->weapon) {
        return true;
    }
    return false;
}

void Stats::update(float dt) {
    if (this->life <= 0) {
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

        Manager::instance().enqueue(entity, OBJECTS);
    }
}