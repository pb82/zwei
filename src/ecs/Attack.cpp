#include <string>

#include "Attack.h"
#include "../Bus.h"
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
#include "Hitbox.h"
#include "../Gfx.h"
#include "Bar.h"

Attack::Attack(Entity &parent) : Component(parent) {}

void Attack::update(float dt) {
    if (wait > 0) {
        wait -= dt;
    }
    if (wait < 0) wait = 0;
}

// You are being attacked
void Attack::defend(std::shared_ptr<Hitbox> hitbox) {
    if (!hitbox->isRanged && hitbox->origin == RT_Context.getPlayer().get()) {
        // If the player turns around before the stick hitbox hits the enemy, then it is not a hit
        auto playerAcc = RT_Context.getPlayer()->getComponent<Acceleration>();
        if (playerAcc->getDirection() != hitbox->launchDirection) {
            return;
        }
    }

    auto acc = this->parent.getComponent<Acceleration>();
    acc->applyForce(hitbox->force);

    auto sprite = this->parent.getComponent<Sprite>();
    sprite->addFilter(std::make_shared<Halo>(500));

    if (this->parent.hasComponent<Stats>()) {
        auto stats = this->parent.getComponent<Stats>();
        stats->character.damage(hitbox->power, hitbox->isRanged);

        auto transform = this->parent.getComponent<Transform>();

        if (!stats->character.dead()) {
            if (this->parent.hasComponent<Bar>()) {
                this->parent.getComponent<Bar>()->setVisibility(1000);
            }
        } else {
            if (&this->parent == RT_Context.getPlayer().get()) {
                Bus::instance().publish(Event(EventPlayerDied));
            }
        }
    }
}

void Attack::attack() {
    if (this->wait > 0) return;
    if (!parent.hasComponent<Stats>()) return;
    auto stats = parent.getComponent<Stats>();
    if (!stats->inventory.hasWeapon()) return;
    this->wait = stats->inventory.weapon->recharge();

    if (stats->inventory.weapon->isRanged()) {
        launchRangedWeapon(stats);
    } else {
        launchMeleeWeapon(stats);
    }
}

std::shared_ptr<Entity> Attack::spawnHitbox(float offsetX, float offsetY, float angle,
                                             Padding padding, std::shared_ptr<Stats> stats) {
    auto acc = parent.getComponent<Acceleration>();
    auto position = parent.getComponent<Transform>();

    auto p = std::make_shared<Entity>();
    p->addComponent<Transform>(position->p.x + offsetX, position->p.y + offsetY);
    p->addComponent<Analytics>();
    auto t = p->getComponent<Transform>();
    p->addComponent<Collider>(t, CT_PROJECTILE, padding);
    p->addComponent<Acceleration>(stats->inventory.weapon->speed(), angle);
    p->addComponent<Hitbox>();

    auto hitbox = p->getComponent<Hitbox>();
    hitbox->power = stats->inventory.weapon->damage(stats->character);
    hitbox->force.set(acc->getAngle(), stats->inventory.weapon->throwback());
    hitbox->isRanged = stats->inventory.weapon->isRanged();
    hitbox->origin = &this->parent;
    hitbox->launchDirection = acc->getDirection();

    return p;
}

void Attack::launchRangedWeapon(std::shared_ptr<Stats> stats) {
    auto animation = parent.getComponent<Animation>();
    auto acc = parent.getComponent<Acceleration>();
    auto direction = acc->getDirection();
    Padding padding = {.75, .75, .75, .75};
    float offsetX = 0, offsetY = 0, angle = 0, rotate = 0;
    animation->queueProjectileFrames();

    switch (direction) {
        case N: angle = VM_50_PI;  offsetY = -.5; rotate = 90;  break;
        case W: angle = VM_100_PI; offsetX = -.5; rotate = -90; break;
        case E: angle = VM_0_PI;   offsetX = .5;  rotate = -90; break;
        case S: angle = VM_150_PI; offsetY = .5;  rotate = 90;  break;
        default: break;
    }

    if (&this->parent != RT_Context.getPlayer().get()) {
        angle = acc->getAngle();
    }
    rotate += angle * (180 / VM_100_PI);

    auto p = spawnHitbox(offsetX, offsetY, angle, padding, stats);

    p->addComponent<Sprite>(SPRITES);
    p->addComponent<Animation>(200, true);
    auto anim = p->getComponent<Animation>();
    anim->addAnimationFrame(stats->inventory.weapon->getProjectileTile());
    anim->rotate = rotate;

    p->addComponent<SelfDestruct>(STILL, 0);

    Force f(angle, 0);
    stats->inventory.weapon->getParams(&f.power, &f.weight, &f.decay);
    p->getComponent<Acceleration>()->applyForce(f);

    Manager::instance().enqueue(p, OBJECTS);
}

void Attack::launchMeleeWeapon(std::shared_ptr<Stats> stats) {
    auto animation = parent.getComponent<Animation>();
    auto acc = parent.getComponent<Acceleration>();
    auto direction = acc->getDirection();
    float offsetX = 0, offsetY = 0, angle = 0;
    Padding padding;
    float range = stats->inventory.weapon->range();

    switch (direction) {
        case N: animation->queueAttackFrames(); offsetY = -1; padding = {1.5, 0, (2 - (range * 2)), 0}; angle = VM_100_PI; break;
        case W: animation->queueAttackFrames(); offsetX = -1; padding = {(2 - (range * 2)), 0, 1.5, 0}; angle = VM_150_PI; break;
        case E: animation->queueAttackFrames(); offsetX =  1; padding = {0, (2 - (range * 2)), 1.5, 0}; angle = VM_150_PI; break;
        case S: animation->queueAttackFrames(); offsetY =  1; padding = {1.5, 0, (2 - (range * 2)), 0}; angle = VM_100_PI; break;
        default: break;
    }

    auto p = spawnHitbox(offsetX, offsetY, angle, padding, stats);
    p->addComponent<SelfDestruct>(DISTANCE, 0.75);
    p->getComponent<Acceleration>()->accelerate();

    Manager::instance().enqueue(p, OBJECTS);
}
