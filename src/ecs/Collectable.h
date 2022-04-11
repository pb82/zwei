#ifndef ZWEI_COLLECTABLE_H
#define ZWEI_COLLECTABLE_H

#include <memory>
#include <functional>

#include "Component.h"
#include "items/Item.h"

class Collectable : public Component {
public:
    Collectable(Entity &parent, std::shared_ptr<Item> item);

    void render(uint8_t) override;

    void collect();

    void onCollect(std::function<void()> cb) {
        this->_onCollect = cb;
    }

    int alpha = 255;

private:
    bool collected = false;

    std::function<void()> _onCollect;

    std::shared_ptr<Item> item;
};


#endif //ZWEI_COLLECTABLE_H
