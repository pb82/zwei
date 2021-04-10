#include "St.h"
#include "snd/Player.h"
#include "io/File.h"
#include "io/Out.h"
#include "in/Input.h"
#include <JSON/printer.h>
#include <JSON/parser.h>

#define SETTINGS_FILE "./settings.json"
#define MUSIC_VOLUME "musicVolume"
#define EFFECTS_VOLUME "effectsVolume"
#define FPS "fps"
#define WINDOW "window"
#define CONTROLS_KEYBOARD "controls_keyboard"
#define CONTROLS_GAMEPAD "controls_gamepad"

St::St()
        : musicVolume(50),
          effectsVolume(50),
          selectedWindowSize(0) {
    supportedSizes.emplace_back(800, 480);
    supportedSizes.emplace_back(800, 600);
    supportedSizes.emplace_back(1024, 640);
    supportedSizes.emplace_back(1024, 800);
    supportedSizes.emplace_back(1280, 640);
    supportedSizes.emplace_back(1280, 800);
    supportedSizes.emplace_back(1280, 1024);
    supportedSizes.emplace_back(1366, 768);
    supportedSizes.emplace_back(1920, 1080);

    supportedFps.push_back(30.0f);
    supportedFps.push_back(60.0f);
}

int St::getMusicVolume() {
    return musicVolume;
}

int St::incMusicVolume() {
    musicVolume += 10;
    if (musicVolume > 100) {
        musicVolume = 100;
    }
    Player::instance().setMusicVolume(musicVolume);
    return musicVolume;
}

int St::decMusicVolume() {
    musicVolume -= 10;
    if (musicVolume < 0) {
        musicVolume = 0;
    }
    Player::instance().setMusicVolume(musicVolume);
    return musicVolume;
}

int St::getEffectsVolume() {
    return this->effectsVolume;
}

int St::incEffectsVolume() {
    effectsVolume += 10;
    if (effectsVolume > 100) {
        effectsVolume = 100;
    }
    Player::instance().setEffectsVolume(effectsVolume);
    return effectsVolume;
}

int St::decEffectsVolume() {
    effectsVolume -= 10;
    if (effectsVolume < 0) {
        effectsVolume = 0;
    }
    Player::instance().setEffectsVolume(effectsVolume);
    return effectsVolume;
}

void St::incWindowSize() {
    selectedWindowSize++;
    selectedWindowSize %= supportedSizes.size();
}

void St::decWindowSize() {
    selectedWindowSize--;
    if (selectedWindowSize < 0) selectedWindowSize = 0;
}

ScreenResolution St::getWindowSize() {
    return supportedSizes.at(selectedWindowSize);
}

void St::getWindowSize(std::string &s) {
    std::stringstream ss;
    ss << supportedSizes.at(selectedWindowSize).w;
    ss << " x ";
    ss << supportedSizes.at(selectedWindowSize).h;
    s = ss.str();
}

void St::incFps() {
    selectedFps++;
    if (selectedFps >= supportedFps.size()) selectedFps = supportedFps.size() - 1;
}

void St::decFps() {
    selectedFps--;
    if (selectedFps < 0) selectedFps = 0;
}

float St::getFps() {
    return supportedFps.at(selectedFps);
}

void St::initAll() {
    Player::instance().setMusicVolume(musicVolume);
    Player::instance().setEffectsVolume(effectsVolume);
}

void St::serializeControls(JSON::Value &target) {
    JSON::Object keyboardControls;
    JSON::Object gamepadControls;

    for (auto &pair : Input::keyboardMapping) {
        keyboardControls[std::to_string(pair.first)] = pair.second;
    }

    for (auto &pair : Input::controllerMapping) {
        gamepadControls[std::to_string(pair.first)] = pair.second;
    }

    target[CONTROLS_KEYBOARD] = keyboardControls;
    target[CONTROLS_GAMEPAD] = gamepadControls;
}

void St::serialize() {
    JSON::Value target;
    JSON::PrettyPrinter p;
    target[MUSIC_VOLUME] = musicVolume;
    target[EFFECTS_VOLUME] = effectsVolume;
    target[FPS] = selectedFps;
    target[WINDOW] = selectedWindowSize;

    serializeControls(target);

    std::string settings = p.print(target);

    Out out(SETTINGS_FILE);
    if (out.open()) {
        out.write(settings);
    }
}

void St::deserializeControls(JSON::Value &source) {
    if (source[CONTROLS_KEYBOARD].is(JSON::JSON_OBJECT)) {
        for (auto &pair : Input::keyboardMapping) {
            auto key = std::to_string(pair.first);
            if (source[CONTROLS_KEYBOARD][key].is(JSON::JSON_NUMBER)) {
                auto val = source[CONTROLS_KEYBOARD][key];
                auto num = static_cast<SDL_Keycode>(std::stoi(key));
                Input::keyboardMapping[num] = static_cast<GameKey>(val.as<int>());
            }
        }
    }
    if (source[CONTROLS_GAMEPAD].is(JSON::JSON_OBJECT)) {
        for (auto &pair : Input::controllerMapping) {
            auto key = std::to_string(pair.first);
            if (source[CONTROLS_GAMEPAD][key].is(JSON::JSON_NUMBER)) {
                auto val = source[CONTROLS_GAMEPAD][key];
                auto num = static_cast<SDL_GameControllerButton>(std::stoi(key));
                Input::keyboardMapping[num] = static_cast<GameKey>(val.as<int>());
            }
        }
    }
}

void St::deserialize() {
    File f(SETTINGS_FILE);
    if (!f.open()) {
        return;
    }

    std::string s;
    s.resize(f.size);
    f.read(s, 0, f.size);

    JSON::Parser parser;
    JSON::Value source;
    parser.parse(source, s);

    if (source[MUSIC_VOLUME].is(JSON::JSON_NUMBER)) {
        this->musicVolume = source[MUSIC_VOLUME].as<int>();
    }
    if (source[EFFECTS_VOLUME].is(JSON::JSON_NUMBER)) {
        this->effectsVolume = source[EFFECTS_VOLUME].as<int>();
    }
    if (source[FPS].is(JSON::JSON_NUMBER)) {
        this->selectedFps = source[FPS].as<int>();
    }
    if (source[WINDOW].is(JSON::JSON_NUMBER)) {
        this->selectedWindowSize = source[WINDOW].as<int>();
    }

    deserializeControls(source);
}