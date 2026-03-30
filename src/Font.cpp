#include "Font.h"
#include "../config.h"

void Font::init() {
    if (font) return;
    int fontSize = static_cast<int>(16 * configZoomFactor);
    font = TTF_OpenFont("assets/RAW/m5x7.ttf", fontSize);
}

TTF_Font *Font::get() {
    if (!font) init();
    return font;
}

int Font::lineHeight() {
    if (!font) init();
    return TTF_FontLineSkip(font);
}
