#ifndef ZWEI_CONTROLLER_H
#define ZWEI_CONTROLLER_H

#include <vector>
#include <map>

#include "Component.h"

class Controller : public Component {
public:
    Controller(Entity &parent);

    void key(GameKeyEvent &key) override;

private:

    void resetKeys();

    float angleFromKeys();

    std::map<GameKey, bool> activeKeys;
};

#endif
