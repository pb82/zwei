#ifndef ZWEI_ENTRYPOINT_H
#define ZWEI_ENTRYPOINT_H

#include "../Scene.h"

class EntryPoint : public Scene {
public:
    EntryPoint();

    void init() override;

    void exit() override;

};


#endif //ZWEI_ENTRYPOINT_H
