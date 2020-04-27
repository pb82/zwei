#include "File.h"

File::~File() {
    if (file.is_open()) {
        file.close();
    }
}

void File::seek(off_t position) {
    file.seekg(position, ios::beg);
}

bool File::open() {
    file.open(path, std::ifstream::in);
    if (file.fail() || !file.is_open()) {
        return false;
    }
    file.seekg(0, ios::end);
    size = file.tellg();
    seek(0);
    return true;
}

bool File::read(string &buffer, off_t position, size_t length) {
    assert(position + length <= size);
    seek(position);
    file.read(&buffer[0], length);
    return !file.eof() && !file.bad() && !file.fail();
}
