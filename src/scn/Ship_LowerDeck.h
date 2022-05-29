#ifndef ZWEI_SHIP_LOWERDECK_H
#define ZWEI_SHIP_LOWERDECK_H

#include "../Scene.h"

class Ship_LowerDeck : public Scene {
public:
    Ship_LowerDeck();

    void init() override;

    void exit() override;
};


#endif //ZWEI_SHIP_LOWERDECK_H
