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
#include "../Draw.h"

Stats::Stats(Entity &parent, bool render) : Component(parent), doRender(render) {}

void Stats::render() {
    if (!doRender) return;
    renderHp();
    this->inventory.render();
}

void Stats::renderHp() {
    auto hp = this->character.getHitpoints();
    std::stringstream ss;
    ss << "*";
    ss << std::get<0>(hp);
    ss << "/";
    ss << std::get<1>(hp);

    auto texture = Assets::instance().getTexture(SPRITES);

    Padding p{0.5, 0.5, 0.5, 0.5};
    SDL_Rect target;
    target.x = 10;
    target.y = 10;
    target.w = 32;
    target.h = 32;

    for (const char c : ss.str()) {
        SDL_Rect source;
        Gfx::pick(source, Text::fromChar(c), texture->w);
        Draw::instance().draw(texture->mem, source, target);
        target.x += 24;
    }

    auto exp = this->character.getExperience();
    ss.str("");
    target.x = 10;
    target.y = 42;
    ss << "^";
    ss << std::get<0>(exp);
    ss << "/";
    ss << std::get<1>(exp);

    for (const char c : ss.str()) {
        SDL_Rect source;
        Gfx::pick(source, Text::fromChar(c), texture->w);
        Draw::instance().draw(texture->mem, source, target);
        target.x += 24;
    }
}

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