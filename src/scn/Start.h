#ifndef ZWEI_START_H
#define ZWEI_START_H

#include "../Scene.h"

class Start : public Scene {
public:
    Start();

    void init() override;

    void exit() override;

    void addEnemies(JSON::Value &resumed) override;

    void addItems(JSON::Value& collected) override;

};


#endif //ZWEI_START_H
