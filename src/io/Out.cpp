#include "Out.h"

Out::~Out() {
    if (file.is_open()) {
        file.close();
    }
}

bool Out::open() {
    file.open(path, std::ofstream::out | std::ofstream::trunc);
    if (file.fail() || !file.is_open()) {
        return false;
    }
    return true;
}

void Out::write(std::string &data) {
    file << data;
    file.flush();
}