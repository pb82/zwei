#ifndef ZWEI_COLLECTABLE_H
#define ZWEI_COLLECTABLE_H

#include <memory>

#include "Component.h"
#include "items/Item.h"

class Collectable : public Component {
public:
    Collectable(Entity &parent, std::shared_ptr<Item> item);

    void render() override;

    void collect();

    int alpha = 255;

private:
    bool collected = false;

    std::shared_ptr<Item> item;
};


#endif //ZWEI_COLLECTABLE_H
