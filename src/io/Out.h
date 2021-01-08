#ifndef ZWEI_OUT_H
#define ZWEI_OUT_H

#include <string>
#include <fstream>

class Out {
public:
    Out(const char *path) : path(path) {}

    ~Out();

    bool open();

    void write(std::string &data);

private:

    std::string path;

    std::ofstream file;

};

#endif //ZWEI_OUT_H
