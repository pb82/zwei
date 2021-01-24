#ifndef ZWEI_CHARACTER_H
#define ZWEI_CHARACTER_H

#include <tuple>

class Character {
public:
    Character();

    bool collectExperience(int points);

    void setLevel(int level);

    void setBase(int hp, int str, int dex, int def);

    int damage(int d, bool isProjectile);

    int getStrength() const;

    int getDexterity() const;

    int releaseExperience() const;

    std::tuple<int, int> getHitpoints();

    std::tuple<int, int> getExperience();

    bool dead() const;

private:

    void adjustToLevel();

    // Experience and level
    int level = 1;
    int experience = 0;
    int experienceToNextLevel = 0;

    // Hitpoints
    int hitpoints = 0;
    int maxHitpoints = 0;
    int baseHitpoints = 0;

    // Stats
    int baseStrength = 0;
    int baseDexterity = 0;
    int baseDefense = 0;
};

#endif //ZWEI_CHARACTER_H
