#ifndef ZWEI_ATTACK_H
#define ZWEI_ATTACK_H

#include "Component.h"
#include "Stats.h"

class Attack : public Component {
public:
    Attack(Entity &parent);

    void update(float dt) override;

    void attack();

private:

    void launchStickWeapon(std::shared_ptr<Stats> stats);

    float wait = 0.0f;
};


#endif //ZWEI_ATTACK_H
