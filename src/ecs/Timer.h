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

    TimerValue(int tile, float cur, float max) {
        this->tile = tile;
        this->max = max;
        this->cur = cur;
    }

    TimerValue(float cur, float max, std::string &&text);

    int tile = 0;
    float max = 0.0f;
    float cur = 0.0f;
    std::vector<int> letters;

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

    void addTimer(int tile, float cur, float max);

    void addTimer(float cur, float max, const char *text);

private:

    std::vector<TimerValue> timers;

};


#endif //ZWEI_TIMER_H
