#ifndef ZWEI_MEM_H
#define ZWEI_MEM_H

#include <JSON/value.h>

class Mem {
public:
    Mem();

    void del(const char *key);

    void setBool(const char *key, bool value);

    bool getBool(const char *key, bool defaultValue);

    void serialize(JSON::Value &to);

private:
    JSON::Value memory;
};

#endif //ZWEI_MEM_H
