#include "Attack.h"
#include "Entity.h"
#include "Animation.h"
#include "Manager.h"
#include "Transform.h"
#include "Collider.h"
#include "Acceleration.h"
#include "NonSprite.h"
#include "SelfDestruct.h"

Attack::Attack(Entity &parent) : Component(parent) {}

void Attack::attack() {
    auto animation = parent.getComponent<Animation>();
    animation->addMixinFrame(17);
    animation->addMixinFrame(32);
    animation->addMixinFrame(24);

    auto position = parent.getComponent<Transform>();
    auto player = parent.getComponent<Acceleration>();


    auto p = std::make_shared<Entity>();
    p->addComponent<Transform>(position->p.x, position->p.y);
    p->addComponent<NonSprite>();

    auto t = p->getComponent<Transform>();
    p->addComponent<Collider>(t, CT_PROJECTILE, 0.4, 0.4);
    p->addComponent<Acceleration>(5.0f, 20, player->trajectory.angle);

    // Self destruct after 5 tiles traveled
    p->addComponent<SelfDestruct>(DISTANCE, 5);

    auto a = p->getComponent<Acceleration>();
    a->go();

    Manager::instance().enqueue(p, OBJECTS);
}