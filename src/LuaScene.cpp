#include "LuaScene.h"
#include "Api.h"
#include "Lighting.h"
#include "ecs/Manager.h"
#include <iostream>

#include "scn/SceneConstants.h"

LuaScene::LuaScene(SceneType type, const std::string& scriptPath)
    : Scene(type), scriptPath(scriptPath) {}

void LuaScene::bindApi() {
    auto zwei = lua.create_table("zwei");

    zwei["init_player"] = []() { Api::initPlayer(); };
    zwei["load_map"] = [](const std::string& file) { Api::loadMap(file.c_str()); };
    zwei["unload_map"] = []() { Api::unloadMap(); };
    zwei["set_player_position"] = [](float x, float y) { Api::setPlayerPosition(x, y); };
    zwei["push_player_position"] = []() { Api::pushPlayerPosition(); };
    zwei["pop_player_position"] = [](sol::this_state ts) -> sol::variadic_results {
        float x, y;
        sol::variadic_results res;
        if (Api::popPlayerPosition(x, y)) {
            res.push_back({ ts, sol::in_place, x });
            res.push_back({ ts, sol::in_place, y });
        }
        return res;
    };
    zwei["set_player_speed"] = [](float speed) { Api::setPlayerSpeed(speed); };
    zwei["set_player_stats"] = [](int hp, int str, int dex, int def) {
        Api::setPlayerStats(hp, str, dex, def);
    };
    zwei["set_game_state"] = []() { Api::setGameState(); };
    zwei["set_enable_lights"] = [](bool enabled) { Api::setEnableLights(enabled); };
    zwei["add_light"] = [](float x, float y, float radius, sol::optional<float> lifetime) -> int {
        return Api::addLight(x, y, radius, lifetime.value_or(0));
    };
    zwei["remove_light"] = [](int id) { Lighting::instance().remove(id); };
    zwei["speech"] = [](const std::string& text) { Api::createSpeechBubble(text.c_str()); };
    zwei["speech_single"] = [](const std::string& text, sol::optional<bool> more) {
        Api::createSingleSpeechBubble(text.c_str(), more.value_or(false));
    };
    zwei["set_trigger"] = [](int x, int y, sol::function onEnter, sol::function onExit, sol::optional<float> proximity) {
        trigger_Fn enterFn = [onEnter](float angle, Entity&) {
            auto res = sol::protected_function(onEnter)(angle);
            if (!res.valid()) {
                sol::error err = res;
                std::cerr << "[LuaScene] Trigger onEnter error: " << err.what() << std::endl;
            }
        };
        trigger_Fn exitFn = [onExit](float angle, Entity&) {
            auto res = sol::protected_function(onExit)(angle);
            if (!res.valid()) {
                sol::error err = res;
                std::cerr << "[LuaScene] Trigger onExit error: " << err.what() << std::endl;
            }
        };
        Api::setTrigger(x, y, enterFn, exitFn, proximity.value_or(0));
    };
}

void LuaScene::init() {
    Manager::instance().collect();
    lua = sol::state();
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    bindApi();

    auto result = lua.safe_script_file(scriptPath, sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;
        std::cerr << "[LuaScene] Error loading " << scriptPath << ": " << err.what() << std::endl;
        return;
    }

    Api::init();
    Api::addItem(10, 27, ItemType::TORCH, []() {});

    sol::protected_function setup = lua["setup"];
    if (setup.valid()) {
        auto res = setup();
        if (!res.valid()) {
            sol::error err = res;
            std::cerr << "[LuaScene] Error in setup(): " << err.what() << std::endl;
        }
    }
}

void LuaScene::exit() {
    sol::protected_function teardown = lua["teardown"];
    if (teardown.valid()) {
        auto res = teardown();
        if (!res.valid()) {
            sol::error err = res;
            std::cerr << "[LuaScene] Error in teardown(): " << err.what() << std::endl;
        }
    }
}

void LuaScene::reload() {
    exit();
    init();
}
