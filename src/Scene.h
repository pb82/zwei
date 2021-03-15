#ifndef ZWEI_SCENE_H
#define ZWEI_SCENE_H

#include <string>

class Scene {
public:
    virtual void init() = 0;

    virtual void exit() = 0;

};

#endif //ZWEI_SCENE_H
