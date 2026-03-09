#ifndef ZWEI_ATTACK_H
#define ZWEI_ATTACK_H

#include "Component.h"
#include "Stats.h"
#include "Hitbox.h"

class Attack : public Component {
public:
    Attack(Entity &parent);

    void update(float dt) override;

    void attack();

    void defend(std::shared_ptr<Hitbox> hitbox);

    float wait = 0.0f;

private:

    std::shared_ptr<Entity> spawnHitbox(float offsetX, float offsetY, float angle,
                                        Padding padding, std::shared_ptr<Stats> stats);

    void launchMeleeWeapon(std::shared_ptr<Stats> stats);

    void launchRangedWeapon(std::shared_ptr<Stats> stats);

};


#endif //ZWEI_ATTACK_H
