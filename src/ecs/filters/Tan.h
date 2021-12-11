#ifndef ZWEI_TAN_H
#define ZWEI_TAN_H

#include "Filter.h"

class Tan : public Filter {
public :
    Tan();

    void render(SDL_Texture *t, const Position *) override;
};


#endif //ZWEI_TAN_H
