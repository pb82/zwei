#ifndef ZWEI_CAM_H
#define ZWEI_CAM_H

#include "../config.h"
#include "alg/Position.h"

// Main and only camera
class Camera {
public:
    Camera()
            : tracked(nullptr),
              w(configWindowWidth),
              h(configWindowHeight),
              z(3.0f) {
    }

    void magnify(float factor) {
        this->z *= abs(factor);
    }

    void zoom(float increment) {
        this->z += increment;
    }

    // rectangle in visible portion of screen?
    bool visible(SDL_Rect &target) const {
        if (!tracked) {
            return false;
        }

        return target.x >= -(configTileSize * z)
               && target.y >= -(configTileSize * z)
               && target.x <= configWindowWidth
               && target.y <= configWindowHeight;
    }

    // adjust the position and size of a rectangle according to the camera position
    // and zoom
    void project(SDL_Rect &target, float x, float y) const {
        if (!tracked) {
            return;
        }

        float f = configTileSize * z;

        // Center on camera
        float dx = configWindowWidth / 2;
        float dy = configWindowHeight / 2;
        float ox = (tracked->x * f) - dx;
        float oy = (tracked->y * f) - dy;

        if (ox < 0) ox = 0;
        if (oy < 0) oy = 0;

        target.x = (x * f) - ox;
        target.y = (y * f) - oy;

        // Calculate the width of the tile by calculating the position of the
        // next tile and then subtracting the current position. This has the
        // advantage of being immune to rounding / off by one pixel errors
        // when zooming
        target.w = ((1 + x) * f - ox) - target.x;
        target.h = ((1 + y) * f - oy) - target.y;
    }

    // set camera position
    void track(Position *p) {
        tracked = p;
    }

private:
    Position *tracked;
    float w;
    float h;
    float z;
};

#endif
