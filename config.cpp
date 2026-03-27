#include "config.h"
#include "src/St.h"

const char *configWindowTitle = "Zwei";
int configWindowWidth = St::instance().getWindowSize().w;
int configWindowHeight = St::instance().getWindowSize().h;
const unsigned int configTileSize = 16;
const int configVirtualWidth = 320;
const int configVirtualHeight = 224;
float configZoomFactor = (float)configWindowWidth / (float)configVirtualWidth;
float configRenderScaleX = 1.0f;
float configRenderScaleY = 1.0f;