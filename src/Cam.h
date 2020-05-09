#ifndef ZWEI_CAM_H
#define ZWEI_CAM_H

#include <algorithm>

#include "../config.h"
#include "alg/Position.h"

// Main and only camera
class Camera {
public:
    Camera()
            : tracked(nullptr),
              w(configWindowWidth),
              h(configWindowHeight),
              z(3.0f),
              mw(0),
              mh(0) {
    }

    void mapSize(int mw, int mh) {
        this->mw = mw;
        this->mh = mh;
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


        // Offsets: the pixels to move in each direction to center in on
        // the tracked sprite
        float dx = std::max((tracked->x * f) + (f / 2) - (configWindowWidth / 2), 0.0f);
        float dy = std::max((tracked->y * f) + (f / 2) - (configWindowHeight / 2), 0.0f);

        dx = std::min(dx, (mw * configTileSize * z) - configWindowWidth);
        dy = std::min(dy, (mh * configTileSize * z) - configWindowHeight);

        // If the zoom factor is out of bounds (more than the whole map can be seen at
        // once) we center the whole map
        if (dx < 0) dx /= 2;
        if (dy < 0) dy /= 2;

        target.x = (x * f) - dx;
        target.y = (y * f) - dy;

        // Calculate the width of the tile by calculating the position of the
        // next tile and then subtracting the current position. This has the
        // advantage of being immune to rounding / off by one pixel errors
        // when zooming
        target.w = ((1 + x) * f - dx) - target.x;
        target.h = ((1 + y) * f - dy) - target.y;
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
    int mw;
    int mh;
};

#endif
