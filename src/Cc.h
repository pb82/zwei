#ifndef ZWEI_CC_H
#define ZWEI_CC_H

#include <queue>
#include <memory>

#include "in/Input.h"

class Command {
public:
    virtual ~Command() {}

    virtual void render() = 0;

    virtual void update(float dt) {}

    virtual void key(GameKeyEvent &ev) {}

    virtual bool done() = 0;
};

class SpeechBubble : public Command {
public:
    SpeechBubble(const char *text, bool last = true);

    ~SpeechBubble() {}

    void render() override;

    void update(float dt) override;

    void key(GameKeyEvent &ev) override;

    bool done();

private:

    float time = 0.0f;

    bool tick = false;

    bool last = true;

    bool read = false;

    std::string text;

    std::vector<int> sequence;
};

#endif
