#ifndef ZWEI_FILE_H
#define ZWEI_FILE_H

#include <fstream>
#include <string>
#include <cassert>

using namespace std;

class File {
public:
    File(std::string path) : path(path) {}

    File(const char *path) : path(path) {}

    ~File();

    void seek(off_t position);

    bool open();

    bool read(string &buffer, off_t position, size_t length);

    off_t size;

private:
    std::string path;
    ifstream file;
};

#endif
