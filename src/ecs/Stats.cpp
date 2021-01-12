#include <sstream>
#include "Stats.h"

#include "Entity.h"
#include "Manager.h"
#include "Sprite.h"
#include "Animation.h"
#include "Transform.h"
#include "SelfDestruct.h"
#include "Group.h"
#include "Acceleration.h"
#include "../alg/Text.h"
#include "../Gfx.h"

Stats::Stats(Entity &parent, bool render) : Component(parent) {}

void Stats::update(float dt) {
    auto hp = this->character.getHitpoints();
    if (std::get<0>(hp) <= 0) {
        // Player gains experience points
        int exp = this->character.releaseExperience();
        auto playerStats = RT_Context.getPlayer()->getComponent<Stats>();

        if (playerStats->character.collectExperience(exp)) {
            // Level up
            auto playerTransform = RT_Context.getPlayer()->getComponent<Transform>();
            float offset = playerTransform->p.x;

            auto s = std::string("level up");
            auto e = std::make_shared<Entity>();
            e->addComponent<Group>();

            auto group = e->getComponent<Group>();

            for (const char c : s) {
                auto letter = std::make_shared<Entity>();
                letter->addComponent<Transform>(
                        offset, playerTransform->p.y,
                        Padding{0.5, 0.5, 0.5, 0.5}
                );
                letter->addComponent<Acceleration>(2.0f, VM_50_PI);
                letter->addComponent<Sprite>(SPRITES);
                letter->addComponent<Animation>(0, false);
                letter->addComponent<SelfDestruct>(TIMER, 500);

                auto anim = letter->getComponent<Animation>();
                anim->addAnimationFrame(Text::fromChar(c));

                auto acc = letter->getComponent<Acceleration>();
                acc->accelerate();

                group->addMember(letter);
                offset += 0.3;
            }

            Manager::instance().enqueue(e, FOREGROUND);
        }

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