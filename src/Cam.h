#ifndef ZWEI_CAM_H
#define ZWEI_CAM_H

#include <algorithm>

#include <SDL2/SDL.h>

#include "../config.h"
#include "alg/Position.h"

// Main and only camera
class Camera {
public:
    Camera()
            : tracked(nullptr),
              w(configWindowWidth),
              h(configWindowHeight),
              z(configZoomFactor),
              mapTilesX(0),
              mapTilesY(0) {}

    void setMapSize(int w, int h) {
        this->mapTilesX = w;
        this->mapTilesY = h;
    }

    void magnify(float factor) {
        if (z >= 5) return;
        this->z *= abs(factor);
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
    void project(SDL_Rect &target, float x, float y, float ox = 0, float oy = 0) const {
        if (!tracked) {
            return;
        }

        // Adjust tile size to zoom factor
        float f = configTileSize * z;

        // Offsets: the pixels to move in each direction to center in on
        // the tracked sprite. Do not scroll past the map size. That's what
        // the max and min are for.
        float dx = std::max((tracked->x * f) + (f / 2) - (configWindowWidth / 2), 0.0f);
        float dy = std::max((tracked->y * f) + (f / 2) - (configWindowHeight / 2), 0.0f);

        dx = std::min(dx, (mapTilesX * configTileSize * z) - configWindowWidth);
        dy = std::min(dy, (mapTilesY * configTileSize * z) - configWindowHeight);

        // If the zoom factor is out of bounds (more than the whole map can be seen at
        // once) we center the whole map
        if (dx < 0) dx /= 2;
        if (dy < 0) dy /= 2;

        // ox and oy are offesets only used for bounding boxes. Remove them if the
        // bounding box should always be the same size as the tile or sprite itself
        target.x = (x * f) - dx + (ox * f / 2);
        target.y = (y * f) - dy + (oy * f);

        // Calculate the width of the tile by calculating the position of the
        // next tile and then subtracting the current position. This has the
        // advantage of being immune to rounding / off by one pixel errors
        // when zooming
        target.w = ((1 + x) * f - dx) - target.x - (ox * f / 2);
        target.h = ((1 + y) * f - dy) - target.y;
    }

    // set camera position
    void track(Position *p) {
        tracked = p;
    }

    float z;

private:
    Position *tracked;
    float w;
    float h;
    int mapTilesX;
    int mapTilesY;
};

#endif
