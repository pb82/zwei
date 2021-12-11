#ifndef ZWEI_BOW_H
#define ZWEI_BOW_H

#include "Weapon.h"

class Bow : public Weapon {
public:
    Bow();

    ~Bow() {}

    float range() override;

    int recharge() override;

    int damage(const Character &c) override;

    float speed() override;

    float throwback() override;

    bool isProjectile() override;

    int getProjectileTile() override;

    void getParams(float *power, float *weight, float *decay) override;

private:
    int damageTable[10] = {2, 3, 3, 3, 3, 3, 4, 5, 6, 8};
};


#endif //ZWEI_BOW_H
