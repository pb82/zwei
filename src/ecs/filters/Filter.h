#ifndef ZWEI_FILTER_H
#define ZWEI_FILTER_H

#include <SDL2/SDL.h>

class Filter {
public:
    Filter(float duration) : duration(0.0f), initial(duration) {}

    bool upate(float dt) {
        duration += dt;
        return duration < initial;
    }

    virtual void render(SDL_Texture *) {
        return;
    };

protected:

    float initial = 0.0f;

    float duration = 0.0f;

};

#endif //ZWEI_FILTER_H
