#ifndef ZWEI_HEART_H
#define ZWEI_HEART_H

#include "Item.h"

class Heart : public Item {
public:
    Heart();

    bool stackable() override;

    int tile() override;

    bool use(std::shared_ptr<Entity> on) override;

    bool useOnPickup() override;

};


#endif //ZWEI_HEART_H
