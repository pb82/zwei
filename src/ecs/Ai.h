#ifndef ZWEI_AI_H
#define ZWEI_AI_H

#include <memory>

#include "Component.h"
#include "../alg/Path.h"

class Ai : public Component {
public:
    Ai(Entity &parent, float delay, std::shared_ptr<Topology> t);

    void update(float dt) override;

    void render() override;

private:

    void plan(float dt);

    float delay;

    float time = 0.0f;

    std::shared_ptr<Topology> topology;

    std::vector<Position> route;

};


#endif
