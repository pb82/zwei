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
#include "../Gfx.h"
#include "../snd/Player.h"
#include "Bar.h"

Attack::Attack(Entity &parent) : Component(parent) {}

void Attack::update(float dt) {
    if (wait > 0) {
        wait -= dt;
    }
    if (wait < 0) wait = 0;
}

// You are being attacked
void Attack::defend(std::shared_ptr<Projectile> projectile) {
    if (!projectile->isProjectile && projectile->origin == RT_Context.getPlayer().get()) {
        // If the player turns around before the stick projectile hits the enemy, then it is not a hit
        auto playerAcc = RT_Context.getPlayer()->getComponent<Acceleration>();
        if (playerAcc->getDirection() != projectile->launchDirection) {
            return;
        }
    }

    auto acc = this->parent.getComponent<Acceleration>();
    acc->applyForce(projectile->force);

    auto sprite = this->parent.getComponent<Sprite>();
    sprite->addFilter(std::make_shared<Halo>(500));

    if (this->parent.hasComponent<Stats>()) {
        auto stats = this->parent.getComponent<Stats>();
        stats->character.damage(projectile->power, projectile->isProjectile);

        auto transform = this->parent.getComponent<Transform>();

        if (!stats->character.dead()) {
            // printDamage(damage, transform->p.x, transform->p.y);
            if (this->parent.hasComponent<Bar>()) {
                // Show enemy health bar for one second
                this->parent.getComponent<Bar>()->setVisibility(1000);
            }
        } else {
            if (&this->parent == RT_Context.getPlayer().get()) {
                RT_Context.state.pushState(GameOver);
                Player::instance().playMusic(MUSIC_GAMEOVER);
            }
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
    if (!stats->inventory.hasWeapon()) return;
    this->wait = stats->inventory.weapon->recharge();

    if (stats->inventory.weapon->isProjectile()) {
        launchProjectileWeapon(stats);
    } else {
        launchStickWeapon(stats);
    }
}

void Attack::launchProjectileWeapon(std::shared_ptr<Stats> stats) {
    auto animation = parent.getComponent<Animation>();
    auto acc = parent.getComponent<Acceleration>();
    auto direction = acc->getDirection();
    Padding padding = {.75, .75, .75, .75};
    float projectileOffsetX = 0, projectileOffsetY = 0, angle = 0, rotate = 0;
    animation->queueProjectileFrames();

    switch (direction) {
        case N:
            angle = VM_50_PI;
            projectileOffsetY = -.5;
            rotate = 90;
            break;
        case W:
            angle = VM_100_PI;
            projectileOffsetX = -.5;
            rotate = -90;
            break;
        case E:
            angle = VM_0_PI;
            projectileOffsetX = .5;
            rotate = -90;
            break;
        case S:
            angle = VM_150_PI;
            projectileOffsetY = .5;
            rotate = 90;
            break;
        default:
            break;
    }

    if (&this->parent != RT_Context.getPlayer().get()) {
        angle = acc->getAngle();
    }
    rotate += angle * (180 / VM_100_PI);

    auto position = parent.getComponent<Transform>();
    auto p = std::make_shared<Entity>();
    p->addComponent<Transform>(
            position->p.x + projectileOffsetX,
            position->p.y + projectileOffsetY);

    p->addComponent<Analytics>();

    auto t = p->getComponent<Transform>();
    p->addComponent<Collider>(t, CT_PROJECTILE, padding);
    p->addComponent<Acceleration>(stats->inventory.weapon->speed(), angle);
    p->addComponent<Projectile>();
    p->addComponent<Sprite>(SPRITES);
    p->addComponent<Animation>(200, true);

    auto anim = p->getComponent<Animation>();
    anim->addAnimationFrame(stats->inventory.weapon->getProjectileTile());
    anim->rotate = rotate;

    auto projectile = p->getComponent<Projectile>();
    projectile->power = stats->inventory.weapon->damage(stats->character);
    projectile->force.set(acc->getAngle(), stats->inventory.weapon->throwback());
    projectile->isProjectile = stats->inventory.weapon->isProjectile();
    projectile->origin = &this->parent;
    projectile->launchDirection = acc->getDirection();

    // Self destruct weapon projectile
    p->addComponent<SelfDestruct>(STILL, 0);

    Force f(angle, 0);
    stats->inventory.weapon->getParams(&f.power, &f.weight, &f.decay);
    p->getComponent<Acceleration>()->applyForce(f);
    p->canSerialize = false;

    Manager::instance().enqueue(p, OBJECTS);
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
            padding.top = (2 - (stats->inventory.weapon->range() * 2));
            angle = VM_100_PI;
            break;
        case W:
            animation->queueAttackFrames();
            projectileOffsetX = -1;
            padding.left = (2 - (stats->inventory.weapon->range() * 2));
            padding.right = 0;
            padding.bottom = 1.5;
            padding.top = 0;
            angle = VM_150_PI;
            break;
        case E:
            animation->queueAttackFrames();
            projectileOffsetX = 1;
            padding.left = 0;
            padding.right = (2 - (stats->inventory.weapon->range() * 2));
            padding.bottom = 1.5;
            padding.top = 0;
            angle = VM_150_PI;
            break;
        case S:
            animation->queueAttackFrames();
            projectileOffsetY = 1;
            padding.left = 1.5;
            padding.right = 0;
            padding.bottom = (2 - (stats->inventory.weapon->range() * 2));
            padding.top = 0;
            angle = VM_100_PI;
            break;
        default:
            break;
    }

    auto position = parent.getComponent<Transform>();
    auto p = std::make_shared<Entity>();
    p->addComponent<Transform>(
            position->p.x + projectileOffsetX,
            position->p.y + projectileOffsetY);

    p->addComponent<Analytics>();

    auto t = p->getComponent<Transform>();
    p->addComponent<Collider>(t, CT_PROJECTILE, padding);
    p->addComponent<Acceleration>(stats->inventory.weapon->speed(), angle);
    p->addComponent<Projectile>();

    auto projectile = p->getComponent<Projectile>();
    projectile->power = stats->inventory.weapon->damage(stats->character);
    projectile->force.set(acc->getAngle(), stats->inventory.weapon->throwback());
    projectile->isProjectile = stats->inventory.weapon->isProjectile();
    projectile->origin = &this->parent;
    projectile->launchDirection = acc->getDirection();

    // Self destruct weapon projectile
    p->addComponent<SelfDestruct>(DISTANCE, 0.75);
    p->getComponent<Acceleration>()->accelerate();
    p->canSerialize = false;

    Manager::instance().enqueue(p, OBJECTS);
}