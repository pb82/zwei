#include "Api.h"

#include "Rt.h"
#include "ecs/Manager.h"
#include "ecs/Menu.h"
#include "ecs/Sprite.h"
#include "ecs/Animation.h"
#include "ecs/Acceleration.h"
#include "ecs/Controller.h"
#include "ecs/Attack.h"
#include "ecs/Interactible.h"
#include "ecs/Collider.h"
#include "ecs/Hud.h"
#include "ecs/Trigger.h"
#include "ecs/Analytics.h"
#include "Cc.h"
#include "ecs/Id.h"
#include "ecs/Ai.h"
#include "ecs/Bar.h"
#include "ecs/arms/Stick.h"
#include "ecs/minds/Kakta.h"
#include "ecs/minds/Ally.h"
#include "ecs/Hostile.h"
#include "ecs/arms/Stone.h"
#include "ecs/minds/Spider.h"
#include "ecs/filters/Tan.h"
#include "ecs/Friend.h"
#include "ecs/Npc.h"
#include "ecs/minds/Caterpillar.h"
#include "ecs/Collectable.h"
#include "ecs/LightSource.h"

namespace Api {

    void init() {
        if (!Manager::instance().hasMenu()) {
            auto menu = Manager::instance().addEntity(UI);
            RT_Context.setMenu(menu);
            menu->addComponent<Menu>();
        }
    }

    void setStartState() {
        RT_State.pushState(StateStart);
    }

    void initPlayer() {
        if (RT_Context.getPlayer() != nullptr) {
            // We still need to add the hud back in
            auto hud = Manager::instance().addEntity(FOREGROUND);
            hud->addComponent<Hud>();
            return;
        }

        // Player
        auto player = Manager::instance().addEntity(OBJECTS);
        RT_Context.setPlayer(player);

        player->addComponent<Sprite>(SPRITES);
        player->addComponent<Animation>(200, true);
        player->addComponent<Controller>();
        player->addComponent<Attack>();
        player->addComponent<Stats>(true);
        player->addComponent<Analytics>();
        player->getComponent<Animation>()->stop();

        // Movement
        player->getComponent<Animation>()->addAnimationFrame(48, 0, 32, 16);
        player->getComponent<Animation>()->addAnimationFrame(49, 1, 33, 17);
        player->getComponent<Animation>()->addAnimationFrame(50, 2, 34, 18);

        // Blunt attack
        player->getComponent<Animation>()->addAttackFrame(51, 3, 35, 19, 100);
        player->getComponent<Animation>()->addAttackFrame(52, 4, 36, 20, 100);
        player->getComponent<Animation>()->addAttackFrame(53, 5, 37, 21, 100);

        // Projectile attack
        player->getComponent<Animation>()->addAttackFrame(54, 6, 38, 22, 100, true);
        player->getComponent<Animation>()->addAttackFrame(55, 7, 39, 23, 100, true);
        player->getComponent<Animation>()->addAttackFrame(56, 8, 40, 24, 100, true);

        // Stats
        auto stats = player->getComponent<Stats>();

        // Hud
        auto hud = Manager::instance().addEntity(FOREGROUND);
        hud->addComponent<Hud>();

        setPlayerSpeed(3);
        setPlayerStats(20, 1, 1, 1);

        player->getComponent<Acceleration>()->setDirection(N);
    }

    void setPlayerPosition(float x, float y, Direction d) {
        if (!RT_Player) return;
        if (!RT_Player->hasComponent<Transform>()) {
            RT_Player->addComponent<Transform>(x, y);

            auto transform = RT_Player->getComponent<Transform>();
            RT_Camera.track(&transform->p);
            RT_Player->addComponent<Collider>(transform, CT_PLAYER);
        } else {
            RT_Player->getComponent<Transform>()->p = {x, y};
        }

        if (RT_Player->hasComponent<Acceleration>()) {
            auto acc = RT_Player->getComponent<Acceleration>();
        }
    }

    void getPlayerPosition(float &x, float &y) {
        if (!RT_Player) return;
        if (RT_Player->hasComponent<Transform>()) {
            auto transform = RT_Player->getComponent<Transform>();
            x = transform->p.x;
            y = transform->p.y;
        }
    }

    void setPlayerSpeed(float maxSpeed) {
        if (!RT_Player) return;
        if (!RT_Player->hasComponent<Acceleration>()) {
            RT_Player->addComponent<Acceleration>(maxSpeed, 0);
        } else {
            RT_Player->getComponent<Acceleration>()->maxSpeed = maxSpeed;
        }
    }

    void setPlayerStats(int hp, int str, int dex, int def) {
        if (!RT_Player) return;
        if (!RT_Player->hasComponent<Stats>()) {
            RT_Player->addComponent<Stats>();
        }
        auto stats = RT_Player->getComponent<Stats>();
        stats->character.setBase(hp, str, dex, def);
    }

    void setMenuState() {
        RT_State.pushState(StateMainMenu);
    }

    void setGameState() {
        RT_State.pushState(StateGame);
    }

    void setGameOverState() {
        RT_State.pushState(StateGameOver);
    }

    bool loadMap(const char *file) {
        Rt_Map.clear();
        RT_Topology.clear();

        if (!Rt_Map.load(file)) {
            return false;
        }

        int x, y;
        Rt_Map.getSize(FLOOR, &x, &y);
        RT_Camera.setMapSize(x, y);
        RT_Topology.make(x, y);
        Rt_Map.topology(RT_Topology.data());
        return true;
    }

    void unloadMap() {
        Rt_Map.clear();
        RT_Topology.clear();
        RT_Camera.setMapSize(0, 0);
        Manager::instance().clearAll();
    }

    void setRoofHideTrigger(int entryX, int entryY) {
        auto trigger = Manager::instance().addEntity(FLOOR);
        trigger->addComponent<Transform>(entryX, entryY);

        auto transform = trigger->getComponent<Transform>();
        trigger->addComponent<Collider>(transform, CT_TRIGGER);
        trigger->addComponent<Trigger>();

        auto handler = trigger->getComponent<Trigger>();
        handler->onEnter([](float, Entity &) {
            Manager::instance().setRenderHint(HINT_HIDE_ROOF_LAYER);
        });
        handler->onExit(nullptr);
    }

    void setRoofShowTrigger(int entryX, int entryY) {
        auto trigger = Manager::instance().addEntity(FLOOR);
        trigger->addComponent<Transform>(entryX, entryY);

        auto transform = trigger->getComponent<Transform>();
        trigger->addComponent<Collider>(transform, CT_TRIGGER);
        trigger->addComponent<Trigger>();

        auto handler = trigger->getComponent<Trigger>();
        handler->onEnter([](float, Entity &) {
            Manager::instance().clearRenderHint(HINT_HIDE_ROOF_LAYER);
        });
        handler->onExit(nullptr);
    }

    void setDoor(int x, int y, uint16_t id) {
        Position p(x, y);
        auto wall = Manager::instance().getWall(p);
        if (wall) {
            wall->addComponent<Interactible>();
            wall->addComponent<Id>(id);
            auto action = wall->getComponent<Interactible>();
            action->onInteract([x, y](Entity &parent) {
                bool accessible = RT_Topology.flipBarrier(x, y);
                parent.getComponent<Collider>()->suspended = accessible;
                if (accessible) {
                    parent.getComponent<Animation>()->queueStateFramesForward();
                } else {
                    parent.getComponent<Animation>()->queueStateFramesBackward();
                }
            });
        }
    }

    void setTrigger(int x, int y, trigger_Fn onEnter, trigger_Fn onExit, float proximity) {
        auto e = std::make_shared<Entity>();
        e->addComponent<Transform>(x, y);
        e->addComponent<Trigger>();

        auto transform = e->getComponent<Transform>();
        e->addComponent<Collider>(transform, CT_TRIGGER, Padding{-proximity, -proximity, -proximity, -proximity});

        auto t = e->getComponent<Trigger>();
        t->onEnter(onEnter);
        t->onExit(onExit);

        Manager::instance().enqueue(e, OBJECTS);
    }

    void setTrigger(int x, int y, trigger_Fn onEnter, trigger_Fn onExit, float extendX, float extendY) {
        auto e = std::make_shared<Entity>();
        e->addComponent<Transform>(x, y);
        e->addComponent<Trigger>();

        auto transform = e->getComponent<Transform>();
        e->addComponent<Collider>(transform, CT_TRIGGER, Padding{0, -extendX, 0, -extendY});

        auto t = e->getComponent<Trigger>();
        t->onEnter(onEnter);
        t->onExit(onExit);

        Manager::instance().enqueue(e, OBJECTS);
    }


    void setInteractible(int x, int y, uint16_t id, interact_Fn onInteract, bool reveal) {
        Position p(x, y);
        auto e = Manager::instance().getWall(p);
        if (!e) {
            return;
        }
        e->addComponent<Interactible>(reveal);
        e->addComponent<Id>(id);
        auto i = e->getComponent<Interactible>();
        i->onInteract(onInteract);
    }

    void createSpeechBubble(const char *text) {
        std::vector<std::shared_ptr<SpeechBubble>> bubbles;
        SpeechBubble::split(text, bubbles);
        for (auto &bubble: bubbles) {
            Rt_Commands.push(bubble);
        }
    }

    void createSingleSpeechBubble(const char *text, bool more) {
        auto b = std::make_shared<SpeechBubble>(text, !more);
        Rt_Commands.push(b);
    }

    void setEnableLights(bool enabled) {
        if (enabled) {
            Manager::instance().clearRenderHint(HINT_TURN_LIGHTS_OUT);
        } else {
            Manager::instance().setRenderHint(HINT_TURN_LIGHTS_OUT);
        }
    }

    void addItem(float x, float y, ItemType type, std::function<void()> cb) {
        auto entity = Item::make({x, y}, type);
        entity->getComponent<Collectable>()->onCollect(cb);
        Manager::instance().enqueue(entity, ITEMS);
    }

    // Enemies

    std::shared_ptr<Entity> addNpc(int x, int y, uint8_t id) {
        auto npc = Manager::instance().addEntity(OBJECTS);
        npc->addComponent<Transform>(x, y);
        npc->addComponent<Sprite>(GIRL);
        npc->addComponent<Animation>(200, true);
        npc->addComponent<Acceleration>(2.0f, 0);
        npc->getComponent<Acceleration>()->setDirection(S);
        npc->addComponent<Id>(id);
        npc->addComponent<Npc>();
        npc->getComponent<Animation>()->addAnimationFrame(12, 0, 8, 4);
        npc->getComponent<Animation>()->addAnimationFrame(13, 1, 9, 5);
        npc->getComponent<Animation>()->addAnimationFrame(14, 2, 10, 6);
        npc->getComponent<Animation>()->stop();
        return npc;
    }

    void addTurnToNpc(std::shared_ptr<Entity> npc, float angle, float duration, float speed) {
        if (!npc->hasComponent<Npc>()) return;

        auto n = npc->getComponent<Npc>();
        n->addTurn(angle, duration, speed);
    }

    void addAlly(int x, int y, uint8_t id, int hp) {
        auto kakta = Manager::instance().addEntity(OBJECTS);
        kakta->addComponent<Transform>(x, y);
        kakta->addComponent<Sprite>(SPRITES);
        kakta->addComponent<Animation>(200, true);
        kakta->addComponent<Acceleration>(3.0f, 0);
        kakta->addComponent<Ai>();
        kakta->addComponent<Attack>();
        kakta->addComponent<Bar>();
        kakta->addComponent<Id>(id);
        kakta->addComponent<Friend>();

        kakta->getComponent<Animation>()->addAnimationFrame(112, 64, 96, 80);
        kakta->getComponent<Animation>()->addAnimationFrame(113, 65, 97, 81);
        kakta->getComponent<Animation>()->addAnimationFrame(114, 66, 98, 82);

        kakta->getComponent<Animation>()->addAttackFrame(115, 67, 99, 83, 300);

        kakta->getComponent<Animation>()->stop();
        kakta->addComponent<Analytics>();

        kakta->addComponent<Attack>();

        kakta->getComponent<Sprite>()->addFilter(std::make_shared<Tan>());

        kakta->addComponent<Stats>(false);
        auto stats = kakta->getComponent<Stats>();
        stats->inventory.equip(std::make_shared<Stone>());
        stats->character.setBase(hp, 2, 10, 5);

        auto transform = kakta->getComponent<Transform>();
        kakta->addComponent<Collider>(transform, CT_ENEMY, Padding{.5, .5, 0.5, 0});
        RT_Topology.registerMobile(&transform->p);

        auto ai = kakta->getComponent<Ai>();
        ai->brainify<Ally>();
    }

    void addSpider(int x, int y, uint8_t id, int hp) {
        auto skeleton = Manager::instance().addEntity(OBJECTS);
        skeleton->addComponent<Transform>(x, y);
        skeleton->addComponent<Sprite>(SPRITES);
        skeleton->addComponent<Animation>(200, true);
        skeleton->addComponent<Acceleration>(4.0f, 0);

        skeleton->addComponent<Ai>();
        skeleton->addComponent<Attack>();

        skeleton->addComponent<Bar>();
        skeleton->addComponent<Id>(id);
        skeleton->addComponent<Hostile>();

        skeleton->getComponent<Animation>()->addAnimationFrame(144);
        skeleton->getComponent<Animation>()->addAnimationFrame(145);
        skeleton->getComponent<Animation>()->addAnimationFrame(146);
        skeleton->getComponent<Animation>()->stop();
        skeleton->addComponent<Analytics>();

        skeleton->addComponent<Stats>(false);
        auto stats = skeleton->getComponent<Stats>();
        stats->inventory.equip(std::make_shared<Stone>());
        stats->character.setBase(hp, 1, 1, 1);

        auto transform = skeleton->getComponent<Transform>();
        skeleton->addComponent<Collider>(transform, CT_ENEMY, Padding{.5, .5, 0.5, 0.7});
        RT_Topology.registerMobile(&transform->p);

        auto ai = skeleton->getComponent<Ai>();
        ai->brainify<Spider>();
    }

    void addCaterpillar(int x, int y, uint8_t id, int hp) {
        auto c = Manager::instance().addEntity(OBJECTS);
        c->addComponent<Transform>(x, y);
        c->addComponent<Sprite>(CATERPILLAR);
        c->addComponent<Animation>(200, true);
        c->addComponent<Acceleration>(2.0f, 0);

        c->addComponent<Ai>();
        c->addComponent<Attack>();

        c->addComponent<Bar>();
        c->addComponent<Id>(id);
        c->addComponent<Hostile>();

        c->getComponent<Animation>()->addAnimationFrame(12, 0, 8, 4);
        c->getComponent<Animation>()->addAnimationFrame(13, 1, 9, 5);
        c->getComponent<Animation>()->addAnimationFrame(14, 2, 10, 6);
        c->getComponent<Animation>()->addAttackFrame(15, 3, 11, 7, 300);
        c->getComponent<Animation>()->stop();
        c->addComponent<Analytics>();

        c->addComponent<Stats>(false);
        auto stats = c->getComponent<Stats>();
        stats->inventory.equip(std::make_shared<Stone>());
        stats->character.setBase(hp, 1, 1, 1);

        auto transform = c->getComponent<Transform>();
        c->addComponent<Collider>(transform, CT_ENEMY, Padding{.5, .5, 0.5, 0.7});
        RT_Topology.registerMobile(&transform->p);

        auto ai = c->getComponent<Ai>();
        ai->brainify<Caterpillar>();
    }

    void addKakta(int x, int y, uint8_t id, int hp) {
        if (!RT_Topology.accessible(x, y)) {
            return;
        }

        auto kakta = Manager::instance().addEntity(OBJECTS);
        kakta->addComponent<Transform>(x, y);
        kakta->addComponent<Sprite>(SPRITES);
        kakta->addComponent<Animation>(200, true);
        kakta->addComponent<Acceleration>(2.0f, 0);
        kakta->addComponent<Ai>();
        kakta->addComponent<Attack>();
        kakta->addComponent<Bar>();
        kakta->addComponent<Id>(id);
        kakta->addComponent<Hostile>();

        kakta->getComponent<Animation>()->addAnimationFrame(112, 64, 96, 80);
        kakta->getComponent<Animation>()->addAnimationFrame(113, 65, 97, 81);
        kakta->getComponent<Animation>()->addAnimationFrame(114, 66, 98, 82);

        kakta->getComponent<Animation>()->addAttackFrame(115, 67, 99, 83, 300);

        kakta->getComponent<Animation>()->stop();
        kakta->addComponent<Analytics>();

        kakta->addComponent<Attack>();

        kakta->addComponent<Stats>(false);
        auto stats = kakta->getComponent<Stats>();
        stats->inventory.equip(std::make_shared<Stick>());
        stats->character.setBase(hp, 5, 10, 1);

        auto transform = kakta->getComponent<Transform>();
        kakta->addComponent<Collider>(transform, CT_ENEMY, Padding{.5, .5, 0.5, 0});
        RT_Topology.registerMobile(&transform->p);

        auto ai = kakta->getComponent<Ai>();
        ai->brainify<Kakta>();
    }

    void addLightSource(std::shared_ptr<Entity> e) {
        if (e) {
            if (!e->hasComponent<LightSource>()) {
                e->addComponent<LightSource>();
                Manager::instance().addLightSource(e);
            }
        }
    }
}