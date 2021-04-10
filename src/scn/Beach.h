#ifndef ZWEI_BEACH_H
#define ZWEI_BEACH_H

#include "../Scene.h"

class Beach : public Scene {
public:
    void init();

    void exit();

    int id() { return 2; }

};


#endif //ZWEI_BEACH_H
