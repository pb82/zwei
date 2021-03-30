#ifndef ZWEI_TORCHMODIFIER_H
#define ZWEI_TORCHMODIFIER_H

#include "Modifier.h"

class TorchModifier : public Modifier {
public:
    TorchModifier(float lifetime);

    void update(float dt) override;

    float modify(float in) override;

private:

    float duration = 0.0f;

};


#endif //ZWEI_TORCHMODIFIER_H
