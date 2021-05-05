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
#include "ecs/Timer.h"
#include "Cc.h"
#include "ecs/Id.h"

namespace Api {

    void init() {
        // Menu
        auto menu = Manager::instance().addEntity(UI);
        RT_Context.setMenu(menu);
        menu->addComponent<Menu>();
    }

    void initPlayer() {
        if (RT_Context.getPlayer() != nullptr) {
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
        setPlayerStats(1, 1, 1, 1);
    }

    void setPlayerPosition(float x, float y, Direction d) {
        if (!RT_Player) return;
        if (!RT_Player->hasComponent<Transform>()) {
            RT_Player->addComponent<Transform>(x, y);

            auto transform = RT_Player->getComponent<Transform>();
            RT_Camera.track(&transform->p);
            RT_Player->addComponent<Collider>(transform, CT_PLAYER,
                                              Padding{.5, .5, 1, 0});
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
    }

    void setRoofHideTrigger(int entryX, int entryY) {
        auto trigger = Manager::instance().addEntity(FLOOR);
        trigger->addComponent<Transform>(entryX, entryY);

        auto transform = trigger->getComponent<Transform>();
        trigger->addComponent<Collider>(transform, CT_TRIGGER);
        trigger->addComponent<Trigger>();

        auto handler = trigger->getComponent<Trigger>();
        handler->onEnter([](float) {
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
        handler->onEnter([](float) {
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

    void setTrigger(int x, int y, trigger_Fn onEnter, trigger_Fn onExit) {
        auto e = std::make_shared<Entity>();
        e->addComponent<Transform>(x, y);
        e->addComponent<Trigger>();

        auto transform = e->getComponent<Transform>();
        e->addComponent<Collider>(transform, CT_TRIGGER);

        auto t = e->getComponent<Trigger>();
        t->onEnter(onEnter);
        t->onExit(onExit);

        Manager::instance().enqueue(e, OBJECTS);
    }

    void setInteractible(int x, int y, uint16_t id, interact_Fn onInteract) {
        Position p(x, y);
        auto e = Manager::instance().getWall(p);
        if (!e) {
            return;
        }
        e->addComponent<Interactible>();
        e->addComponent<Id>(id);
        auto i = e->getComponent<Interactible>();
        i->onInteract(onInteract);
    }

    void createSpeechBubble(const char *text) {
        std::vector<std::shared_ptr<SpeechBubble>> bubbles;
        SpeechBubble::split(text, bubbles);
        for (auto &bubble : bubbles) {
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
}