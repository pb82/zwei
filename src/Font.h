#ifndef ZWEI_FONT_H
#define ZWEI_FONT_H

#include <SDL_ttf.h>

class Font {
public:
    static Font &instance() {
        static Font instance;
        return instance;
    }

    Font(Font const &) = delete;
    void operator=(Font const &) = delete;

    void init();
    TTF_Font *get();
    int lineHeight();
    void shutdown();

private:
    Font() {}
    TTF_Font *font = nullptr;
};

#endif
