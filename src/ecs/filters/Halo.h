#ifndef ZWEI_HALO_H
#define ZWEI_HALO_H

#include "Filter.h"

class Halo : public Filter{
public :
    Halo(float duration);

    void render(SDL_Texture *t) override;
};


#endif //ZWEI_HALO_H
