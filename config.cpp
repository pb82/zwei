#include "config.h"
#include "src/St.h"

const char *configWindowTitle = "Zwei";
const int configWindowWidth = St::instance().getWindowSize().w;
const int configWindowHeight = St::instance().getWindowSize().h;
const unsigned int configTileSize = 16;
const float configTargetFramerate = St::instance().getFps();
const float configZoomFactor = 4.0f;