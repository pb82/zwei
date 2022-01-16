#ifndef ZWEI_INTERACTIBLE_H
#define ZWEI_INTERACTIBLE_H

#include <functional>

#include <JSON/value.h>

#include "Component.h"


typedef std::function<void(Entity &)> interact_Fn;

class Interactible : public Component {
public:
    Interactible(Entity &parent, bool reveal = false);

    void onInteract(interact_Fn cb);

    void update(float dt) override;

    void render(uint8_t) override;

    void interact();

private:

    uint8_t alpha = 0;

    bool reveal = false;

    interact_Fn fn;

};


#endif //ZWEI_INTERACTIBLE_H
