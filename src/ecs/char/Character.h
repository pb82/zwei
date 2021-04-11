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

    std::tuple<int, int> getHitpoints();

    bool dead() const;

    void serialize(JSON::Value &to);

private:

    // Hitpoints
    int hitpoints = 0;
    int maxHitpoints = 0;

    // Stats
    int baseStrength = 0;
    int baseDexterity = 0;
    int baseDefense = 0;
};

#endif //ZWEI_CHARACTER_H
