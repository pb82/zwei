#ifndef ZWEI_NPC_H
#define ZWEI_NPC_H

#include "Component.h"

#include <queue>
#include <functional>

struct NpcTurn {
    NpcTurn(float angle, float duration, float speed) : angle(angle), duration(duration), speed(speed) {}

    float angle;
    float duration;
    float speed;
};

class Npc : public Component {
public:
    Npc(Entity &parent);

    void update(float dt) override;

    void addTurn(float angle, float duration = 1000, float speed = 1.0f);

    bool done();

private:

    bool _done = false;

    std::queue<NpcTurn> turns;

};


#endif //ZWEI_NPC_H
