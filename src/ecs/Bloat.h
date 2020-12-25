#ifndef ZWEI_BLOAT_H
#define ZWEI_BLOAT_H

#include "Component.h"

class Bloat : public Component {
public:
    Bloat(Entity &paret);

    void update(float dt) override;
};


#endif //ZWEI_BLOAT_H
