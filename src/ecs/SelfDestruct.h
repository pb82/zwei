#ifndef ZWEI_SELFDESTRUCT_H
#define ZWEI_SELFDESTRUCT_H

#include "Component.h"

enum SelfDestructType {
    TIMER,
    DISTANCE,
    STILL,
};

class SelfDestruct : public Component {
public:
    SelfDestruct(Entity &parent, SelfDestructType type, float value);

    void update(float dt) override;

    float value;

    bool serialize(JSON::Value &to) override;

    std::string name() { return "selfDestruct"; }

private:

    void updateDistance(float dt);

    SelfDestructType type;

    float current = 0.0f;

};


#endif //ZWEI_SELFDESTRUCT_H
