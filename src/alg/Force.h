#ifndef ZWEI_FORCE_H
#define ZWEI_FORCE_H

class Force {
public:
    Force(float angle, float power);

    Force(float angle, float power, float decay, float weight);

    bool apply(float *x, float *y);

    void update(float dt);

    void set(float angle, float power);

    float decay = 100.0f;

    float weight = 100.0f;

    float power;

private:

    float angle;

};

#endif
