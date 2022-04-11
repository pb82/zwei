#ifndef ZWEI_TEST_H
#define ZWEI_TEST_H

#include "../Scene.h"

class Test : public Scene {
public:
    Test();

    void init() override;

    void exit() override;

private:

    void tryExit();

};


#endif //ZWEI_TEST_H
