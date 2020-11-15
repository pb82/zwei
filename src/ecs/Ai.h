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

    void collide(std::shared_ptr<Collider> other);

    void render() override;

    template<typename T>
    void brainify() {
        this->mind = std::make_shared<T>(this->parent);
    }

private:

    float time = 0.0f;

    std::shared_ptr<Mind> mind;

};

#endif
