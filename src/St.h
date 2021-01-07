#ifndef ZWEI_ST_H
#define ZWEI_ST_H

class St {
public:
    static St &instance() {
        static St instance;
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

    void initAll();

private:
    St() : musicVolume(50), effectsVolume(50) {}

    int musicVolume;

    int effectsVolume;

};

#endif //ZWEI_ST_H
