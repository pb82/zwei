#ifndef ZWEI_FRIEND_H
#define ZWEI_FRIEND_H

#include "Component.h"

class Friend : public Component {
public:
    Friend(Entity &parent) : Component(parent) {}
};

#endif //ZWEI_FRIEND_H
