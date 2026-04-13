#include "Ctx.h"

#include <JSON/printer.h>

#include "ecs/Stats.h"
#include "io/Out.h"
#include "ecs/Acceleration.h"
#include "ecs/Animation.h"
#include "ecs/Interactible.h"
#include "ecs/Collider.h"
#include "scn/Entry.h"
#include "LuaScene.h"
#include "Lighting.h"

GameStateMachine::GameStateMachine() {
    this->current.push(StateStart);
}

GameState GameStateMachine::currentState() {
    return this->current.top();
}

void GameStateMachine::popState() {
    if (this->current.size() > 1) this->current.pop();
}

bool GameStateMachine::toggleMenu() {
    // Can't get out of the initial start menu by pressing esc
    if (this->current.top() == StateStart) return false;
    if (this->current.top() == StateLoading) return false;

    // In-game allow to toggle the menu
    if (this->current.top() == StateMainMenu) {
        this->current.pop();
    } else {
        this->current.push(StateMainMenu);
        return true;
    }
    return false;
}

void GameStateMachine::pushState(GameState state) {
    if (this->currentState() != state) {
        this->current.push(state);
    }
}

Ctx::Ctx() {
    scenes.emplace(SceneEntry, std::make_shared<Entry>());
    scenes.emplace(SceneForest, std::make_shared<LuaScene>(SceneForest, "./scenes/forest.lua"));
}

void Ctx::setPlayer(std::shared_ptr<Entity> player) {
    this->player = player;
}

void Ctx::setMenu(std::shared_ptr<Entity> menu) {
    this->menu = menu;
}

std::shared_ptr<Entity> Ctx::getPlayer() {
    return this->player;
}

std::shared_ptr<Entity> Ctx::getMenu() {
    return this->menu;
}

void Ctx::reloadActiveScene() {
    if (!activeScene) return;
    auto* luaScene = dynamic_cast<LuaScene*>(activeScene.get());
    if (!luaScene) return;
    setActiveScene(activeScene->getSceneType());
}

void Ctx::autosave() {
    auto stats = getPlayer()->getComponent<Stats>();
    auto acc = getPlayer()->getComponent<Acceleration>();
    auto t = getPlayer()->getComponent<Transform>();

    JSON::Value to;
    to["scene"] = activeScene->getSceneType();
    stats->inventory.serialize(to);
    stats->character.serialize(to);
    Lighting::instance().serialize(to);
    this->memory.serialize(to);

    to["x"] = t->p.x;
    to["y"] = t->p.y;
    to["direction"] = acc->getDirection();

    Out f("savegame.json");
    if (f.open()) {
        JSON::PrettyPrinter p;
        std::string result = p.print(to);
        f.write(result);

        if (!saved) {
            saved = new bool;
        }
        *saved = true;
    }
}

bool Ctx::savegameExists() {
    if (!saved) {
        File f("savegame.json");
        saved = new bool;
        *saved = f.open();
    }
    return *saved;
}

void Ctx::load(float *x, float *y) {
    File f("savegame.json");
    if (f.open()) {
        std::string buffer;
        buffer.resize(f.size);
        f.read(buffer, 0, f.size);

        JSON::Value v;
        JSON::Parser p;
        p.parse(v, buffer);

        SceneType sceneType = static_cast<SceneType>(v["scene"].as<int>());
        this->memory.deserialize(v);

        setActiveScene(sceneType);

        auto stats = this->getPlayer()->getComponent<Stats>();
        stats->character.deserialize(v);
        stats->inventory.deserialize(v);

        auto t = this->getPlayer()->getComponent<Transform>();
        Lighting::instance().deserialize(v, &t->p);

        *x = v["x"].as<float>();
        *y = v["y"].as<float>();

        Direction d = static_cast<Direction>(v["direction"].as<int>());
        auto acc = RT_Context.getPlayer()->getComponent<Acceleration>();
        acc->setDirection(d);
    }
}