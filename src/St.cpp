#include "St.h"
#include "snd/Player.h"
#include "io/File.h"
#include "io/Out.h"
#include <JSON/printer.h>
#include <JSON/parser.h>

#define SETTINGS_FILE "./settings.json"
#define MUSIC_VOLUME "musicVolume"
#define EFFECTS_VOLUME "effectsVolume"
#define FPS "fps"
#define WINDOW "window"

St::St()
        : musicVolume(50),
          effectsVolume(50),
          selectedWindowSize(0) {
    supportedSizes.push_back({800, 480});
    supportedSizes.push_back({800, 600});
    supportedSizes.push_back({1024, 640});
    supportedSizes.push_back({1024, 800});
    supportedSizes.push_back({1280, 800});
    supportedSizes.push_back({1280, 1024});
    supportedSizes.push_back({1366, 768});
    supportedSizes.push_back({1920, 1080});

    supportedFps.push_back(60.0f);
    supportedFps.push_back(30.0f);
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

void St::incFps() {
    selectedFps++;
    selectedFps %= supportedFps.size();
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

void St::serialize() {
    JSON::Value target;
    JSON::PrettyPrinter p;
    target[MUSIC_VOLUME] = musicVolume;
    target[EFFECTS_VOLUME] = effectsVolume;
    target[FPS] = selectedFps;
    target[WINDOW] = selectedWindowSize;

    std::string settings = p.print(target);

    Out out(SETTINGS_FILE);
    if (out.open()) {
        out.write(settings);
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
}