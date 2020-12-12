#ifndef ZWEI_FORCE_H
#define ZWEI_FORCE_H

class Force {
public:
    Force(float angle, float power);

    bool apply(float weight, float *x, float *y);

    void update(float weight, float dt);

    void set(float angle, float power);

private:

    float angle;

    float power;
};

#endif
