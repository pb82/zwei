#include "Cc.h"

void SpeechBubble::key(GameKeyEvent &ev) {
    if (!ev.valid) return;
    if (ev.state != GK_PUSHED) return;
    if (ev.key == GK_A) {
        this->read = true;
    }
}

bool SpeechBubble::done() {
    return read;
}

void SpeechBubble::render() {

}