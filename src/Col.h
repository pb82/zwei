#ifndef ZWEI_COL_H
#define ZWEI_COL_H

#include <vector>
#include <memory>

#include "ecs/Collider.h"

static bool intersects(SDL_Rect &a, SDL_Rect &b);

class Col {
public:

    static void collide();

};

#endif
