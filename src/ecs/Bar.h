#ifndef ZWEI_BAR_H
#define ZWEI_BAR_H

#include "Component.h"

enum BarType {
    LIVE,
};

class Bar : public Component {
public:
    Bar(Entity &parent);

    void render() override;
};


#endif //ZWEI_BAR_H
