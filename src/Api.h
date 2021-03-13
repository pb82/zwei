#ifndef ZWEI_API_H
#define ZWEI_API_H


namespace Api {
    // Init
    void init();

    void setPlayerSpeed(float maxSpeed);

    void setPlayerPosition(float x, float y);

    void setPlayerStats(int hp, int str, int dex, int def);

    // State
    void setMenuState();

    void setGameState();

    void setGameOverState();

    // Maps
    bool loadMap(const char *file);

    void unloadMap();

    void setRoofHideTrigger(int entryX, int entryY);

    void setRoofShowTrigger(int entryX, int entryY);

    void setDoor(int x, int y);

}

#endif //ZWEI_API_H
