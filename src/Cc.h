#ifndef ZWEI_CC_H
#define ZWEI_CC_H

#include <queue>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <string>

#include "in/Input.h"
#include "Scene.h"
#include "alg/Color.h"
#include "ecs/Entity.h"

class Command {
public:
    virtual ~Command() {}

    virtual void render() = 0;

    virtual void update(float dt) {}

    virtual void key(GameKeyEvent &ev) {}

    virtual bool done() = 0;
};

class NpcCommand : public Command {
public:
    NpcCommand();

    ~NpcCommand();

    void addNpc(std::shared_ptr<Entity> npc);

    bool done();

    void render() override;

    void update(float dt) override;

private:
    std::vector<std::shared_ptr<Entity>> npcs;

    bool finished = false;
};

class ScreenTransition : public Command {
public:

    ScreenTransition(SceneType newScene);

    void render();

    void update(float dt);

    bool done();

private:

    bool switched = false;

    int frames = 0;

    SDL_Rect r;

    Color c;

    bool ready = false;

    SceneType newScene;

};

class SpeechBubble : public Command {
public:
    SpeechBubble(const char *text, bool last = true);

    ~SpeechBubble();

    static void split(const char *text, std::vector<std::shared_ptr<SpeechBubble>> &target);

    static int getWrapWidth();
    static int getLineHeight();

    void render() override;

    void update(float dt) override;

    void key(GameKeyEvent &ev) override;

    bool done();

    bool last = true;

private:

    float time = 0.0f;

    float charTimer = 0.0f;

    bool tick = false;

    bool read = false;

    int visibleChars = 0;

    std::string text;

    SDL_Texture *textTexture = nullptr;
    int textW = 0;
    int textH = 0;

    void buildTexture();
};

#endif
