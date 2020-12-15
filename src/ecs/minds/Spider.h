#ifndef ZWEI_SPIDER_H
#define ZWEI_SPIDER_H

#include "Mind.h"

class Spider : public Mind {
public:

    Spider(Entity &parent);

    ~Spider();

    int delay() override;

    void plan(float dt) override;

    bool activate() override;

private:

    std::vector<Position> route;

    Position goal;

    float activationRange = 5.0f;

};


#endif //ZWEI_SPIDER_H
