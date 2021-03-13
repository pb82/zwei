#ifndef ZWEI_INTERACTIBLE_H
#define ZWEI_INTERACTIBLE_H

#include <functional>

#include "Component.h"

typedef std::function<void(Entity &)> interact_Fn;

class Interactible : public Component {
public:
    Interactible(Entity &parent);

    void onInteract(interact_Fn cb);

    void update(float dt) override;

    void interact();

private:

    interact_Fn fn;

};


#endif //ZWEI_INTERACTIBLE_H
