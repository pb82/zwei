#ifndef ZWEI_ATTACK_H
#define ZWEI_ATTACK_H

#include "Component.h"
#include "Stats.h"
#include "Projectile.h"

class Attack : public Component {
public:
    Attack(Entity &parent);

    void update(float dt) override;

    void attack();

    void defend(std::shared_ptr<Projectile> projectile);

private:

    void printDamage(float damage, float x, float y);

    void launchStickWeapon(std::shared_ptr<Stats> stats);

    int frameForChar(char c);

    float wait = 0.0f;
};


#endif //ZWEI_ATTACK_H
