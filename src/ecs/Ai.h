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

    void render(uint8_t) override;

    void pause(float duration);

    template<typename T>
    void brainify() {
        this->mind = std::make_shared<T>(this->parent);
    }

    void brainify(std::shared_ptr<Mind> mind) {
        this->mind = mind;
    }

    std::shared_ptr<Mind> getMind() {
        return this->mind;
    }

private:

    float _pause = 0.0f;

    float time = 0.0f;

    std::shared_ptr<Mind> mind;

};

#endif
