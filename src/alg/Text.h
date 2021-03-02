#ifndef ZWEI_TEXT_H
#define ZWEI_TEXT_H

#include <string>
#include <vector>

struct Text {
    static int fromChar(char c);

    static int fromInt(int c);

    static void toSequence(std::string &s, std::vector<int> &target);

};


#endif //ZWEI_TEXT_H
