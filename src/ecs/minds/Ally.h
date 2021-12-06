#ifndef ZWEI_ALLY_H
#define ZWEI_ALLY_H

#include "Mind.h"

class Ally : public Mind {
public:
    Ally(Entity &parent);

    ~Ally();

    int delay() override;

    void plan(float dt) override;

    bool activate() override;

private:

    std::vector<Position> route;

    float activationRange = 10.0f;

};


#endif //ZWEI_ALLY_H
