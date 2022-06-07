#ifndef ZWEI_LIGHTSOURCE_H
#define ZWEI_LIGHTSOURCE_H

#include "Component.h"


class LightSource : public Component {
public:
    LightSource(Entity &parent);

    LightSource(Entity &parent, float lifetime);

    void update(float dt) override;

    uint8_t getAlpha(float d);

private:

    bool expires = false;

    float lifetime = 0.0f;

    float age = 0.0f;

};


#endif //ZWEI_LIGHTSOURCE_H
