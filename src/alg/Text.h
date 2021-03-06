#ifndef ZWEI_TEXT_H
#define ZWEI_TEXT_H

#include <string>
#include <vector>

struct Text {
    static int fromChar(char c);

    static int fromInt(int c);

    static void toSequence(const char *s, std::vector<int> &target);

private:

    static int nextWhitespace(const char *s, int from);
};


#endif //ZWEI_TEXT_H
