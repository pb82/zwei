#ifndef ZWEI_NONSPRITE_H
#define ZWEI_NONSPRITE_H

#include <ASSETS/Assets.h>

#include "Component.h"

class NonSprite : public Component {
public:
    NonSprite(Entity &parent);

    void render() override;

    void update(float dt) override;
};


#endif //ZWEI_NONSPRITE_H
