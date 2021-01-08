#ifndef ZWEI_ST_H
#define ZWEI_ST_H

#include <JSON/value.h>

struct ScreenResolution {
    ScreenResolution(int w, int h)
            : w(w), h(h) {}

    int w;
    int h;
};

class St {
public:
    static St &instance() {
        static St instance;
        if (!instance.fresh) {
            instance.deserialize();
            instance.fresh = true;
        }
        return instance;
    }

    St(St const &) = delete;

    void operator=(St const &) = delete;

    int getMusicVolume();

    int incMusicVolume();

    int decMusicVolume();

    int getEffectsVolume();

    int incEffectsVolume();

    int decEffectsVolume();

    void incWindowSize();

    void decWindowSize();

    ScreenResolution getWindowSize();

    void incFps();

    void decFps();

    float getFps();

    void initAll();

    void serialize();

    void deserialize();

private:
    St();

    int musicVolume;

    int effectsVolume;

    int selectedWindowSize;

    int selectedFps;

    std::vector<ScreenResolution> supportedSizes;

    std::vector<float> supportedFps;

    bool fresh = false;
};

#endif //ZWEI_ST_H
