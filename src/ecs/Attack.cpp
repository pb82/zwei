#include <string>

#include "Attack.h"
#include "Entity.h"
#include "Animation.h"
#include "Manager.h"
#include "Transform.h"
#include "Collider.h"
#include "Acceleration.h"
#include "SelfDestruct.h"
#include "Sprite.h"
#include "filters/Halo.h"
#include "Analytics.h"
#include "Projectile.h"
#include "Group.h"
#include "../alg/Text.h"

Attack::Attack(Entity &parent) : Component(parent) {}

void Attack::update(float dt) {
    if (wait > 0) {
        wait -= dt;
    }
    if (wait < 0) wait = 0;
}

void Attack::printDamage(float damage, float x, float y) {
    std::string number = std::to_string((int) damage);
    float offset = x - ((number.size() * 0.3) / 2);

    auto container = std::make_shared<Entity>();
    container->addComponent<Group>();

    auto group = container->getComponent<Group>();

    for (const char digit : number) {
        auto entity = std::make_shared<Entity>();
        entity->addComponent<Transform>(offset, y, Padding{0.5, 0.5, 0.5, 0.5});
        entity->addComponent<Acceleration>(2.0f, VM_50_PI);
        entity->addComponent<Sprite>(SPRITES);
        entity->addComponent<Animation>(0, false);
        entity->addComponent<SelfDestruct>(TIMER, 500);

        auto animation = entity->getComponent<Animation>();
        animation->addAnimationFrame(Text::fromChar(digit));

        auto acceleration = entity->getComponent<Acceleration>();
        acceleration->accelerate();
        group->addMember(entity);
        offset += 0.3;
    }
    Manager::instance().enqueue(container, FOREGROUND);
}

// You are being attacked
void Attack::defend(std::shared_ptr<Projectile> projectile) {
    auto acc = this->parent.getComponent<Acceleration>();
    acc->applyForce(projectile->force);

    auto sprite = this->parent.getComponent<Sprite>();
    sprite->addFilter(std::make_shared<Halo>(500));

    if (this->parent.hasComponent<Stats>()) {
        auto stats = this->parent.getComponent<Stats>();
        int damage = stats->character.damage(projectile->power, projectile->isProjectile);

        auto transform = this->parent.getComponent<Transform>();

        if (!stats->character.dead()) {
            printDamage(damage, transform->p.x, transform->p.y);
        }
    }
}

void Attack::attack() {
    // Weapon needs recharge?
    if (this->wait > 0) return;

    // Entity must have the ability to equip weapons
    if (!parent.hasComponent<Stats>()) return;
    auto stats = parent.getComponent<Stats>();

    // Entity must have a weapon equiped
    if (!stats->hasWeapon()) return;
    this->wait = stats->weapon->recharge();

    if (stats->weapon->isProjectile()) {
        // TODO
    } else {
        launchStickWeapon(stats);
    }
}

void Attack::launchStickWeapon(std::shared_ptr<Stats> stats) {
    auto animation = parent.getComponent<Animation>();
    auto acc = parent.getComponent<Acceleration>();

    auto direction = acc->getDirection();
    float projectileOffsetX = 0, projectileOffsetY = 0, angle = 0;
    Padding padding;

    switch (direction) {
        case N:
            animation->queueAttackFrames();
            projectileOffsetY = -1;
            padding.left = 1.5;
            padding.right = 0;
            padding.bottom = 0;
            padding.top = (2 - (stats->weapon->range() * 2));
            angle = VM_100_PI;
            break;
        case W:
            animation->queueAttackFrames();
            projectileOffsetX = -1;
            padding.left = (2 - (stats->weapon->range() * 2));
            padding.right = 0;
            padding.bottom = 1.5;
            padding.top = 0;
            angle = VM_150_PI;
            break;
        case E:
            animation->queueAttackFrames();
            projectileOffsetX = 1;
            padding.left = 0;
            padding.right = (2 - (stats->weapon->range() * 2));
            padding.bottom = 1.5;
            padding.top = 0;
            angle = VM_150_PI;
            break;
        case S:
            animation->queueAttackFrames();
            projectileOffsetY = 1;
            padding.left = 1.5;
            padding.right = 0;
            padding.bottom = (2 - (stats->weapon->range() * 2));
            padding.top = 0;
            angle = VM_100_PI;
            break;
    }

    auto position = parent.getComponent<Transform>();
    auto sprite = parent.getComponent<Sprite>();

    auto p = std::make_shared<Entity>();
    p->addComponent<Transform>(
            position->p.x + projectileOffsetX,
            position->p.y + projectileOffsetY);

    p->addComponent<Analytics>();

    auto t = p->getComponent<Transform>();
    p->addComponent<Collider>(t, CT_PROJECTILE, padding);
    p->addComponent<Acceleration>(stats->weapon->speed(), angle);
    p->addComponent<Projectile>();

    auto projectile = p->getComponent<Projectile>();
    projectile->power = stats->weapon->damage(stats->character);
    projectile->force.set(acc->getAngle(), stats->weapon->throwback());
    projectile->isProjectile = stats->weapon->isProjectile();
    projectile->origin = &this->parent;

    // Self destruct weapon projectile
    p->addComponent<SelfDestruct>(DISTANCE, 0.75);
    p->getComponent<Acceleration>()->accelerate();

    Manager::instance().enqueue(p, OBJECTS);
}