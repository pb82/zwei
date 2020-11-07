#ifndef ZWEI_ATTACK_H
#define ZWEI_ATTACK_H

#include "Component.h"

class Attack : public Component {
public:
    Attack(Entity& parent);

    void attack();
};


#endif //ZWEI_ATTACK_H
