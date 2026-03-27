#ifndef ZWEI_CONFIG_H
#define ZWEI_CONFIG_H

extern const char *configWindowTitle;
extern int configWindowWidth;
extern int configWindowHeight;
extern const unsigned int configTileSize;
extern float configZoomFactor;
extern float configRenderScaleX;
extern float configRenderScaleY;

// Fixed virtual resolution — determines how much of the map is visible,
// independent of window size. Styled after SNES-era action RPGs.
extern const int configVirtualWidth;
extern const int configVirtualHeight;

#endif
