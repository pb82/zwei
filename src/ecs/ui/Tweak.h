#ifndef ZWEI_TWEAK_H
#define ZWEI_TWEAK_H

#include <memory>

#include "../Component.h"

class Tweak : public Component {
public:
    Tweak(Entity &parent, std::shared_ptr<Entity> client);

    void render(uint8_t) override;

private:
    std::shared_ptr<Entity> client;
};


#endif
