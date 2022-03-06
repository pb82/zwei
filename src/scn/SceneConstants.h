#ifndef ZWEI_SCENECONSTANTS_H
#define ZWEI_SCENECONSTANTS_H

#define GET_BOOL(c)     RT_Memory.getBool(c, false)
#define SET_BOOL(c, b)  RT_Memory.setBool(c, b)

namespace SceneConstants {
    extern const char *KEY_IntroductionDialog;
    extern const char *KEY_OnTheRun;

    // Misc
    extern const uint16_t ID_InteractibleNotes;
    extern const uint16_t ID_Npc_1;
    extern const uint16_t ID_Npc_2;

    // Enemies
    extern const uint16_t ID_Enemy_Kakta_1;
    extern const uint16_t ID_Enemy_Kakta_2;
    extern const uint16_t ID_Enemy_Kakta_3;
    extern const uint16_t ID_Enemy_Kakta_4;

    // Doors
    extern const uint16_t ID_Door_1;
}

#endif //ZWEI_SCENECONSTANTS_H
