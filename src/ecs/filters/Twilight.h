#ifndef ZWEI_TWILIGHT_H
#define ZWEI_TWILIGHT_H

#include <memory>

#include "Filter.h"
#include "../../alg/Position.h"


class Twilight : public Filter {
public :
    Twilight(Position *tracked);

    void render(SDL_Texture *t, const Position *) override;

private:

    Position *tracked;
};


#endif //ZWEI_TWILIGHT_H
