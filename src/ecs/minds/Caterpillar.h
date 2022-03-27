#ifndef ZWEI_CATERPILLAR_H
#define ZWEI_CATERPILLAR_H

#include "Mind.h"

class Caterpillar : public Mind {
public:
    Caterpillar(Entity &parent);

    ~Caterpillar();

    int delay() override;

    void plan(float dt) override;

    bool activate() override;

private:

    std::vector<Position> route;

    Position goal;

    float activationRange = 4.0f;

};


#endif //ZWEI_CATERPILLAR_H
