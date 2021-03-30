#ifndef ZWEI_TIMER_H
#define ZWEI_TIMER_H

#include <vector>

#include "Component.h"

struct TimerValue {
    TimerValue(int tile, float max) {
        this->tile = tile;
        this->max = max;
        this->cur = 0.0f;
    }

    int tile = 0;
    float max = 0.0f;
    float cur = 0.0f;

    bool running() {
        return cur < max;
    }

    void update(float dt) {
        this->cur += dt;
    }
};

class Timer : public Component {
public:
    Timer(Entity &parent);

    void render(uint8_t) override;

    void update(float dt) override;

    void addTimer(int tile, float max);

private:

    std::vector<TimerValue> timers;

};


#endif //ZWEI_TIMER_H
