#ifndef ZWEI_INTERACTIBLE_H
#define ZWEI_INTERACTIBLE_H

#include <functional>

#include <JSON/value.h>

#include "Component.h"


typedef std::function<void(Entity &)> interact_Fn;

class Interactible : public Component {
public:
    Interactible(Entity &parent, uint8_t id);

    void onInteract(interact_Fn cb);

    void update(float dt) override;

    void interact();

    uint8_t id;

private:

    interact_Fn fn;

};


#endif //ZWEI_INTERACTIBLE_H
