#ifndef ZWEI_AI_H
#define ZWEI_AI_H

#include <memory>

#include "Component.h"
#include "Collider.h"
#include "minds/Mind.h"

#include "../alg/Path.h"

class Ai : public Component {
public:
    Ai(Entity &parent);

    void update(float dt) override;

    void render() override;

    void pause(float duration);

    template<typename T>
    void brainify() {
        this->mind = std::make_shared<T>(this->parent);
    }

private:

    float _pause = 0.0f;

    float time = 0.0f;

    std::shared_ptr<Mind> mind;

};

#endif
