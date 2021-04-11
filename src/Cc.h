#ifndef ZWEI_CC_H
#define ZWEI_CC_H

#include <queue>
#include <memory>
#include <typeindex>
#include <typeinfo>

#include "in/Input.h"
#include "Scene.h"
#include "alg/Color.h"

class Command {
public:
    virtual ~Command() {}

    virtual void render() = 0;

    virtual void update(float dt) {}

    virtual void key(GameKeyEvent &ev) {}

    virtual bool done() = 0;
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
    SpeechBubble(std::vector<int> &sequence, bool last = true);

    SpeechBubble(const char *text, bool last = true);

    ~SpeechBubble() {}

    static void split(const char *text, std::vector<std::shared_ptr<SpeechBubble>> &target);

    void render() override;

    void update(float dt) override;

    void key(GameKeyEvent &ev) override;

    bool done();

    bool last = true;

private:

    float time = 0.0f;

    float lastIndex = 0.0f;

    bool tick = false;

    bool read = false;

    int index = 0;

    std::vector<int> sequence;
};

#endif
