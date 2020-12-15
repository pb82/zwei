#ifndef ZWEI_KAKTA_H
#define ZWEI_KAKTA_H

#include "Mind.h"

class Kakta : public Mind {
public:

    Kakta(Entity &parent);

    ~Kakta();

    int delay() override;

    void plan(float dt) override;

    bool activate() override;

private:

    std::vector<Position> route;

    Position goal;

    // You need to get closer than 3 tiles for it to activate
    float activationRange = 10.0f;

};

#endif
