#include "Mem.h"

Mem::Mem() {
    memory = JSON::Object{};
}

void Mem::serialize(JSON::Value &to) {
    to["memory"] = this->memory;
}

void Mem::deserialize(JSON::Value &from) {
    this->memory = from["memory"];
}

void Mem::del(const char *key) {
    memory[key] = JSON::null;
}

void Mem::setBool(const char *key, bool value) {
    memory[key] = value;
}

bool Mem::getBool(const char *key, bool defaultValue) {
    auto value = memory[key];
    if (!value.is(JSON::JSON_BOOL)) {
        return defaultValue;
    }
    return value.as<bool>();
}

void Mem::addToArray(const char *key, uint8_t id) {
    auto arr = memory[key];
    if (arr.is(JSON::JSON_NULL)) {
        JSON::Array targetArray;
        memory[key] = targetArray;
    }

    memory[key].asMutable<JSON::Array>().push_back(id);
}

bool Mem::arrayContains(const char *key, uint8_t id) {
    auto arr = memory[key];
    if (arr.is(JSON::JSON_NULL)) {
        return false;
    }

    for (auto item : arr.as<JSON::Array>()) {
        if (item.as<uint8_t>() == id) {
            return true;
        }
    }
    return false;
}