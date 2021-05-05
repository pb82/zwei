#ifndef ZWEI_ID_H
#define ZWEI_ID_H

#include "Component.h"

class Id : public Component {
public:
    Id(Entity &parent, uint16_t id) : Component(parent), id(id) {}

    uint16_t id;

};


#endif //ZWEI_ID_H
