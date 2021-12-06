#ifndef ZWEI_TEST_H
#define ZWEI_TEST_H

#include "../Scene.h"

class Test : public Scene {
public:
    Test();

    void init() override;

    void exit() override;

    void addEnemies(JSON::Value &resumed) override;

    void addItems(JSON::Value &collected) override;

};


#endif //ZWEI_TEST_H
