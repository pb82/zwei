#ifndef ZWEI_TRIGGER_H
#define ZWEI_TRIGGER_H

#include <functional>

#include "Component.h"

typedef std::function<void(float angle)> trigger_Fn;

class Trigger : public Component {
public:
    Trigger(Entity &parent);

    void onEnter(trigger_Fn cb);

    void onExit(trigger_Fn cb);

    void update(float dt) override;

private:

    trigger_Fn enter;

    trigger_Fn exit;

    bool active = false;

};


#endif //ZWEI_TRIGGER_H
