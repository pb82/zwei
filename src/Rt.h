#ifndef ZWEI_RT_H
#define ZWEI_RT_H

#include <queue>

#include "Cam.h"
#include "Ctx.h"
#include "in/Input.h"
#include "Cc.h"
#include "Map.h"

#define RT_Running Rt::instance().running
#define RT_Stop Rt::instance().stop
#define RT_Camera Rt::instance().camera
#define RT_Context Rt::instance().context
#define RT_Input Rt::instance().in
#define Rt_Commands Rt::instance().commands
#define Rt_Map Rt::instance().map
#define RT_Topology RT_Context.getTopology()
#define RT_Player RT_Context.getPlayer()
#define RT_Menu RT_Context.getMenu()
#define RT_Memory RT_Context.getMemory()
#define RT_State RT_Context.state

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
    Map map;

    std::queue<std::shared_ptr<Command>> commands;

    void stop() {
        this->running = false;
    }

private:
    Rt() : running(true), map("./assets/NEW/MAPS") {}
};

#endif
