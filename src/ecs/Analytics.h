#ifndef ZWEI_ANALYTICS_H
#define ZWEI_ANALYTICS_H

#include "Component.h"

class Analytics : public Component {
public:
    Analytics(Entity &parent);

    void render(uint8_t) override;

    void update(float dt) override;

};


#endif //ZWEI_ANALYTICS_H
