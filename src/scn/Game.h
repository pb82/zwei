//
// Created by peter on 11.02.24.
//

#ifndef ZWEI_GAME_H
#define ZWEI_GAME_H

#include "../Scene.h"

class Game : public Scene {
public:
    Game();

    void init() override;

    void exit() override;

};


#endif //ZWEI_GAME_H
