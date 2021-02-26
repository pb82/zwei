#ifndef ZWEI_CC_H
#define ZWEI_CC_H

#include <queue>
#include <memory>

#include "in/Input.h"

class Command {
public:
    virtual ~Command() {}

    virtual void render() = 0;

    virtual void key(GameKeyEvent &ev) {}

    virtual bool done() = 0;
};

class SpeechBubble : public Command {
public:
    SpeechBubble(const char *text);

    ~SpeechBubble() {}

    void render();

    void key(GameKeyEvent &ev);

    bool done();

private:

    bool read = false;

    std::string text;
};

#endif
