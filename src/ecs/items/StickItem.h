#ifndef ZWEI_STICKITEM_H
#define ZWEI_STICKITEM_H

#include "Item.h"

class StickItem : public Item {
public:
    StickItem();

    bool stackable() override;

    int tile() override;

    bool canEquip() override;
    
    void equip(std::shared_ptr<Entity> on) override;

    void unequip(std::shared_ptr<Entity> on) override;

};


#endif //ZWEI_STICKITEM_H
