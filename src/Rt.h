#ifndef ZWEI_RT_H
#define ZWEI_RT_H

#include <queue>

#include "Cam.h"
#include "Ctx.h"
#include "in/Input.h"
#include "Cc.h"

#define RT_Running Rt::instance().running
#define RT_Stop Rt::instance().stop
#define RT_Camera Rt::instance().camera
#define RT_Context Rt::instance().context
#define RT_Input Rt::instance().in
#define Rt_Commands Rt::instance().commands

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
    Ctx context;
    Input in;

    std::queue<std::shared_ptr<Command>> commands;

    void stop() {
        this->running = false;
    }

private:
    Rt() : running(true) {}
};

#endif
