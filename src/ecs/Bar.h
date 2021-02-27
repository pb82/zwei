#ifndef ZWEI_BAR_H
#define ZWEI_BAR_H

#include "Component.h"
#include "../alg/Padding.h"

enum BarType {
    LIVE,
};

class Bar : public Component {
public:
    Bar(Entity &parent);

    void render() override;

    void update(float dt) override;

    void setVisibility(float duration);

private:

    float visible = 0.0f;

};


#endif //ZWEI_BAR_H
