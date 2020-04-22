#ifndef ZWEI_CAM_H
#define ZWEI_CAM_H

// Main and only camera
class Camera {
public:
    Camera() : x(0.0f), y(0.0f), z(1.0f) {}

    void magnify(float factor) {
        this->z *= factor;
    }

    void zoom(float increment) {
        this->z += increment;
    }

    void track(float x, float y) {
        this->x = x;
        this->y = y;
    }

    float x;
    float y;
    float z;
};

#endif
