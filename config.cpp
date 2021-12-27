#include "config.h"
#include "src/St.h"

const char *configWindowTitle = "Zwei";
const int configWindowWidth = St::instance().getWindowSize().w;
const int configWindowHeight = St::instance().getWindowSize().h;
const unsigned int configTileSize = 16;
const float configZoomFactor = 5.0f;
float configRenderScaleX = 1.0f;
float configRenderScaleY = 1.0f;