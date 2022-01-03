#ifndef ZWEI_CHARACTER_H
#define ZWEI_CHARACTER_H

#include <JSON/value.h>

#include <tuple>

class Character {
public:
    Character();

    void setBase(int hp, int str, int dex, int def);

    int damage(int d, bool isProjectile);

    int getStrength() const;

    int getDexterity() const;

    void bumpHitpoints();

    std::tuple<int, int> getHitpoints();

    std::tuple<int, int> getHitpointsPerHeart();

    std::tuple<int, int> getHearts();

    bool dead() const;

    void serialize(JSON::Value &to);

    void deserialize(JSON::Value &from);

private:

    // Hitpoints
    int hitpoints = 0;
    int maxHitpoints = 0;

    // Stats
    int baseStrength = 0;
    int baseDexterity = 0;
    int baseDefense = 0;

    const int BAR_HITPOINTS = 20;
};

#endif //ZWEI_CHARACTER_H
