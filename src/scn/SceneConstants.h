#ifndef ZWEI_SCENECONSTANTS_H
#define ZWEI_SCENECONSTANTS_H

#define GET_BOOL(c)     RT_Memory.getBool(c, false)
#define SET_BOOL(c, b)  RT_Memory.setBool(c, b)

namespace SceneConstants {
    const char *KEY_IntroductionDialog = "key.beach.intro";
    const char *KEY_OnTheRun = "key.beach.ontherun";

    // Misc
    const uint16_t ID_InteractibleNotes = 0;
    const uint16_t ID_Npc_1 = 1;
    const uint16_t ID_Npc_2 = 2;

    // Enemies
    const uint16_t ID_Enemy_Kakta_1 = 100;
    const uint16_t ID_Enemy_Kakta_2 = 101;
    const uint16_t ID_Enemy_Kakta_3 = 103;
    const uint16_t ID_Enemy_Kakta_4 = 104;

    // Doors
    const uint16_t ID_Door_1 = 105;
}

#endif //ZWEI_SCENECONSTANTS_H
