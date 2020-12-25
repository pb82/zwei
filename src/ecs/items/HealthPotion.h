#ifndef ZWEI_HEALTHPOTION_H
#define ZWEI_HEALTHPOTION_H

#include "Item.h"

class HealthPotion : public Item {
public:
    HealthPotion();

    bool stackable() override;

    int tile() override;

};


#endif //ZWEI_HEALTHPOTION_H
