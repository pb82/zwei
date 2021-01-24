#ifndef ZWEI_BOWITEM_H
#define ZWEI_BOWITEM_H

#include "Item.h"

class BowItem : public Item {
public:
    BowItem();

    bool stackable() override;

    int tile() override;

    bool canEquip() override;

    void equip(std::shared_ptr<Entity> on) override;

    void unequip(std::shared_ptr<Entity> on) override;

};


#endif //ZWEI_BOWITEM_H
