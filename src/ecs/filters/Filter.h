#ifndef ZWEI_FILTER_H
#define ZWEI_FILTER_H

#include <SDL2/SDL.h>

#include "../../alg/Position.h"

class Filter {
public:
    Filter() : duration(1), initial(1), permanent(true) {}

    Filter(float duration) : duration(0.0f), initial(duration), permanent(false) {}

    bool upate(float dt) {
        if (permanent) return true;
        duration += dt;
        return duration < initial;
    }

    virtual void render(SDL_Texture *, const Position *) {
        return;
    };

protected:

    float initial = 0.0f;

    float duration = 0.0f;

private:

    bool permanent = false;

};

#endif //ZWEI_FILTER_H
