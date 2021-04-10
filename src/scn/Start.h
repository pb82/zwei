#ifndef ZWEI_START_H
#define ZWEI_START_H

#include "../Scene.h"

class Start : public Scene {
public:
    void init();

    void exit();

    int id() { return 1; }

};


#endif //ZWEI_START_H
