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

    float wait = 0.0f;

private:

    void launchStickWeapon(std::shared_ptr<Stats> stats);

    void launchProjectileWeapon(std::shared_ptr<Stats> stats);

};


#endif //ZWEI_ATTACK_H
