#ifndef ZWEI_FORCE_H
#define ZWEI_FORCE_H

class Force {
public:
    Force(float angle, float power);

    void apply(float weight, float *x, float *y);

    void update(float dt);

private:

    float angle;

    float power;
};

#endif
