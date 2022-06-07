#ifndef ZWEI_API_H
#define ZWEI_API_H

#include "ecs/Trigger.h"
#include "ecs/Interactible.h"
#include "alg/Direction.h"
#include "ecs/items/Item.h"

namespace Api {
    // Init
    void init();

    void initPlayer();

    void setPlayerSpeed(float maxSpeed);

    void setPlayerPosition(float x, float y, Direction = N);

    void getPlayerPosition(float &x, float &y);

    void setPlayerStats(int hp, int str, int dex, int def);

    // State
    void setMenuState();

    void setGameState();

    void setGameOverState();

    // Maps

    bool loadMap(const char *file);

    void unloadMap();

    void setRoofHideTrigger(int entryX, int entryY);

    void setRoofShowTrigger(int entryX, int entryY);

    void setDoor(int x, int y, uint16_t id);

    void setTrigger(int x, int y, trigger_Fn onEnter, trigger_Fn onExit, float proximity = 0);

    void setTrigger(int x, int y, trigger_Fn onEnter, trigger_Fn onExit, float extendX, float extendY);

    void setInteractible(int x, int y, uint16_t id, interact_Fn onInteract, bool reveal = false);

    void createSpeechBubble(const char *text);

    void createSingleSpeechBubble(const char *text, bool more);

    // Enemies

    void addKakta(int x, int y, uint8_t id, int hp = 20);

    void addSpider(int x, int y, uint8_t id, int hp = 10);

    void addCaterpillar(int x, int y, uint8_t id, int hp = 10);

    void addAlly(int x, int y, uint8_t id, int hp = 20);

    std::shared_ptr<Entity> addNpc(int x, int y, uint8_t id);

    void addTurnToNpc(std::shared_ptr<Entity> npc, float angle, float duration, float speed = 2.0f);

    // Items

    void addItem(float x, float y, ItemType type, std::function<void()> cb = nullptr);

    // Effects

    void setEnableLights(bool enabled);

    void addLightSource(std::shared_ptr<Entity> e);

}

#endif //ZWEI_API_H
