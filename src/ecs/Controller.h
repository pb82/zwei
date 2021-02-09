#ifndef ZWEI_CONTROLLER_H
#define ZWEI_CONTROLLER_H

#include <memory>
#include <vector>
#include <map>

#include "Component.h"

class Controller : public Component {
public:
    Controller(Entity &parent);

    void key(GameKeyEvent &key) override;

    void resetKeys();

private:

    void attack();

    void drop();

    void use();

    void lock(bool locked = true);

    void inv(bool prev = false);

    void stop();

    void go();

    void updateState(GameKeyEvent &key);

    float angleFromKeys();

    std::map<GameKey, bool> activeKeys;
};

#endif
