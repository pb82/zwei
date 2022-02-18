#ifndef ZWEI_ENTRY_H
#define ZWEI_ENTRY_H

#include "../Scene.h"

class Entry : public Scene {
public:
    Entry();

    void init() override;

    void exit() override;
};


#endif //ZWEI_ENTRY_H
