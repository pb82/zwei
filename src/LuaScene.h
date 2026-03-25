#ifndef ZWEI_LUASCENE_H
#define ZWEI_LUASCENE_H

#include <string>
#include <sol/sol.hpp>
#include "Scene.h"

class LuaScene : public Scene {
public:
    LuaScene(SceneType type, const std::string& scriptPath);
    void init() override;
    void exit() override;
    void reload();

private:
    std::string scriptPath;
    sol::state lua;
    void bindApi();
};

#endif
