#ifndef ZWEI_NPC_H
#define ZWEI_NPC_H

#include "Mind.h"

#include <functional>

struct NpcTurn {
    NpcTurn(float angle, float duration, float speed = 1.0f) : angle(angle),
                                                               duration(duration), speed(speed) {}

    float angle;
    float duration;
    float speed;
};

class Npc : public Mind {
public:
    Npc(Entity &parent);

    void plan(float dt) override;

    bool activate() override;

    virtual int delay() override;

    void addTurn(float angle, float duration, float speed = 1.0f);

    void onFinish(std::function<void()> cb);

    void go();

private:
    bool start = false;

    int index = 0;

    std::function<void()> _finish;

    std::vector<std::shared_ptr<NpcTurn>> turns;
};


#endif //ZWEI_NPC_H
