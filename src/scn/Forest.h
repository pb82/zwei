#ifndef ZWEI_FOREST_H
#define ZWEI_FOREST_H

#include "../Scene.h"

class Forest : public Scene {
public:
    Forest();

    void init() override;

    void exit() override;

};


#endif //ZWEI_FOREST_H
