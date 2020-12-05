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

Attack::Attack(Entity &parent) : Component(parent) {}

void Attack::attack() {
    auto animation = parent.getComponent<Animation>();
    auto acc = parent.getComponent<Acceleration>();

    auto direction = acc->getDirection();

    switch (direction) {
        case N:
            animation->addMixinFrame(51);
            animation->addMixinFrame(52);
            animation->addMixinFrame(53);
            break;
        case W:
            animation->addMixinFrame(19);
            animation->addMixinFrame(20);
            animation->addMixinFrame(21);
            break;
        case E:
            animation->addMixinFrame(35);
            animation->addMixinFrame(36);
            animation->addMixinFrame(37);
            break;
        case S:
            animation->addMixinFrame(3);
            animation->addMixinFrame(4);
            animation->addMixinFrame(5);
            break;
    }

    auto position = parent.getComponent<Transform>();
    auto player = parent.getComponent<Acceleration>();
    auto sprite = parent.getComponent<Sprite>();

    auto p = std::make_shared<Entity>();
    p->addComponent<Transform>(position->p.x, position->p.y);
    p->addComponent<Analytics>();

    auto t = p->getComponent<Transform>();
    p->addComponent<Collider>(t, CT_PROJECTILE, 0.4, 0.4);
    p->addComponent<Acceleration>(5.0f, player->getAngle());

    // Self destruct after 5 tiles traveled
    p->addComponent<SelfDestruct>(DISTANCE, 5);

    auto a = p->getComponent<Acceleration>();
    a->accelerate();

    Manager::instance().enqueue(p, OBJECTS);
}