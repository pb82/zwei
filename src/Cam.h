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
              z(5.0f) {
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

        target.x = x * (configTileSize * z) - tracked->x;
        target.y = y * (configTileSize * z) - tracked->y;
        target.w = configTileSize * z;
        target.h = configTileSize * z;
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
