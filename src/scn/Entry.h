#ifndef ZWEI_ENTRY_H
#define ZWEI_ENTRY_H

#include "../Scene.h"

class Entry : public Scene {
public:
    void init();

    void exit();

    int id() { return 0; }

};


#endif //ZWEI_ENTRY_H
