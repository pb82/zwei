#ifndef ZWEI_CONTROLLER_H
#define ZWEI_CONTROLLER_H

#include "Component.h"

class Controller : public Component {
public:
    Controller(Entity &parent);

    void key(GameKeyEvent &key) override;
};

#endif
