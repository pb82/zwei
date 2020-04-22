#ifndef ZWEI_RT_H
#define ZWEI_RT_H

#include "Cam.h"

#define RT_Running Rt::instance().running
#define RT_Stop Rt::instance().stop

// Shared runtime state
class Rt {
public:
    static Rt &instance() {
        static Rt instance;
        return instance;
    }

    Rt(Rt const &) = delete;

    void operator=(Rt const &) = delete;

    bool running;
    Camera camera;

    void stop() {
        this->running = false;
    }

private:
    Rt() : running(true) {}
};

#endif
