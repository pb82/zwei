#ifndef ZWEI_SCENE_H
#define ZWEI_SCENE_H

#include <string>

enum SceneType {
    SceneStart,
    SceneDungeon,
    SceneBeach
};

class Scene {
public:

    Scene(SceneType t) {
        this->type = t;
    }

    virtual void init() = 0;

    virtual void exit() = 0;

    virtual ~Scene() {}

    SceneType getSceneType() { return this->type; }

private:

    SceneType type;
};

#endif //ZWEI_SCENE_H
