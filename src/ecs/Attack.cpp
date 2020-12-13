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

Attack::Attack(Entity &parent) : Component(parent) {}

void Attack::update(float dt) {
    if (wait > 0) {
        wait -= dt;
    }
    if (wait < 0) wait = 0;
}

// You are being attacked
void Attack::defend(std::shared_ptr<Projectile> projectile) {
    auto acc = this->parent.getComponent<Acceleration>();
    acc->applyForce(projectile->force);

    auto sprite = this->parent.getComponent<Sprite>();
    sprite->addFilter(std::make_shared<Halo>(500));

    if (this->parent.hasComponent<Stats>()) {
        auto stats = this->parent.getComponent<Stats>();
        stats->life -= projectile->power;
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
            animation->addMixinFrame(51);
            animation->addMixinFrame(52);
            animation->addMixinFrame(53);
            projectileOffsetY = -1;
            padding.left = 1.5;
            padding.right = 0;
            padding.bottom = 0;
            padding.top = (2 - (stats->weapon->range() * 2));
            angle = VM_100_PI;
            break;
        case W:
            animation->addMixinFrame(19);
            animation->addMixinFrame(20);
            animation->addMixinFrame(21);
            projectileOffsetX = -1;
            padding.left = (2 - (stats->weapon->range() * 2));
            padding.right = 0;
            padding.bottom = 1.5;
            padding.top = 0;
            angle = VM_150_PI;
            break;
        case E:
            animation->addMixinFrame(35);
            animation->addMixinFrame(36);
            animation->addMixinFrame(37);
            projectileOffsetX = 1;
            padding.left = 0;
            padding.right = (2 - (stats->weapon->range() * 2));
            padding.bottom = 1.5;
            padding.top = 0;
            angle = VM_150_PI;
            break;
        case S:
            animation->addMixinFrame(3);
            animation->addMixinFrame(4);
            animation->addMixinFrame(5);
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
    projectile->power = stats->weapon->power();
    projectile->force.set(acc->getAngle(), stats->weapon->throwback());

    // Self destruct weapon projectile
    p->addComponent<SelfDestruct>(DISTANCE, 0.75);
    p->getComponent<Acceleration>()->accelerate();

    Manager::instance().enqueue(p, OBJECTS);
}