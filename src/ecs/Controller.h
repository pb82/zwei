#ifndef ZWEI_CONTROLLER_H
#define ZWEI_CONTROLLER_H

#include <vector>
#include <map>

#include "Component.h"

#define P_UP (activeKeys[GK_UP] == true)
#define P_DOWN (activeKeys[GK_DOWN] == true)
#define P_LEFT (activeKeys[GK_LEFT] == true)
#define P_RIGHT (activeKeys[GK_RIGHT] == true)

class Controller : public Component {
public:
    Controller(Entity &parent);

    void key(GameKeyEvent &key) override;

private:

    // void addKey(GameKey key);

    // void removeKey(GameKey key);

    float angleFromKeys(float angle);

    // std::vector<GameKey> activeKeys;

    std::map<GameKey, bool> activeKeys;
};

#endif
