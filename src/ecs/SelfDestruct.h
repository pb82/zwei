#ifndef ZWEI_SELFDESTRUCT_H
#define ZWEI_SELFDESTRUCT_H

#include "Component.h"

enum SelfDestructType {
    TIMER,
    DISTANCE
};

class SelfDestruct : public Component {
public:
    SelfDestruct(Entity &parent, SelfDestructType type, float value);

    void update(float dt) override;

private:

    void updateDistance(float dt);

    SelfDestructType type;

    float value;

    float current = 0.0f;

};


#endif //ZWEI_SELFDESTRUCT_H
