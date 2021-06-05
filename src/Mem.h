#ifndef ZWEI_MEM_H
#define ZWEI_MEM_H

#include <JSON/value.h>

class Mem {
public:
    Mem();

    void del(const char *key);

    void setBool(const char *key, bool value);

    bool getBool(const char *key, bool defaultValue);

    void addToArray(const char *key, uint8_t id);

    bool arrayContains(const char *key, uint8_t id);

    void serialize(JSON::Value &to);

    void deserialize(JSON::Value &from);

private:
    JSON::Value memory;
};

#endif //ZWEI_MEM_H
