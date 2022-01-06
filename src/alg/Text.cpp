#include "Text.h"

#include <cstring>

int Text::nextWhitespace(const char *s, int from) {
    int x = 0;
    int len = std::strlen(s);
    for (int i = from; i < len; i++) {
        char c = s[i];
        if (c == 32 || c == 9 || c == 10) break;
        x += 14;
    }
    return x;
}

void Text::toSequence(const char *s, std::vector<int> &target) {
    target.clear();
    int maxx = 800 - 28;
    int maxy = 240 - 18;
    int x = 0;
    int y = 0;
    int last;
    int len = std::strlen(s);


    for (int i = 0; i < len; i++) {
        // Y wrap
        last = fromChar(s[i]);
        if (y >= maxy) {
            target.push_back(-2);
            y = 0;
        }

        target.push_back(last);

        // Whitespace. Check if we should line-break here
        if (last == 0) {
            int lookahead = nextWhitespace(s, i + 1);
            if (x + lookahead > maxx) {
                // Insert line break
                target.push_back(-1);
                x = 0;
                y += 18;
                continue;
            }
        }

        // Word too long but no line-break was possible
        if (x >= maxx) {
            if (last != 0) {
                target.push_back(13);
            }
            target.push_back(-1);
            y += 18;
            x = 0;
        }

        x += 14;
    }
}

int Text::fromInt(int c) {
    switch (c) {
        case 0:
            return 16;
        case 1:
            return 17;
        case 2:
            return 18;
        case 3:
            return 19;
        case 4:
            return 20;
        case 5:
            return 21;
        case 6:
            return 22;
        case 7:
            return 23;
        case 8:
            return 24;
        case 9:
            return 25;
        default:
            return 16;
    }
}

int Text::fromChar(char c) {
    switch (c) {
        case ' ':
            return 0;
        case '!':
            return 1;
        case '&':
            return 6;
        case '\'':
            return 7;
        case '(':
            return 8;
        case ')':
            return 9;
        case '*':
            return 10;
        case ',':
            return 12;
        case '-':
            return 13;
        case '.':
            return 14;
        case '/':
            return 15;
        case '?':
            return 31;
        case '0':
            return 16;
        case '1':
            return 17;
        case '2':
            return 18;
        case '3':
            return 19;
        case '4':
            return 20;
        case '5':
            return 21;
        case '6':
            return 22;
        case '7':
            return 23;
        case '8':
            return 24;
        case '9':
            return 25;
        case ':':
            return 26;
        case 'a':
            return 65;
        case 'b':
            return 66;
        case 'c':
            return 67;
        case 'd':
            return 68;
        case 'e':
            return 69;
        case 'f':
            return 70;
        case 'g':
            return 71;
        case 'h':
            return 72;
        case 'i':
            return 73;
        case 'j':
            return 74;
        case 'k':
            return 75;
        case 'l':
            return 76;
        case 'm':
            return 77;
        case 'n':
            return 78;
        case 'o':
            return 79;
        case 'p':
            return 80;
        case 'q':
            return 81;
        case 'r':
            return 82;
        case 's':
            return 83;
        case 't':
            return 84;
        case 'u':
            return 85;
        case 'v':
            return 86;
        case 'w':
            return 87;
        case 'x':
            return 88;
        case 'y':
            return 89;
        case 'z':
            return 90;
        case 'A':
            return 33;
        case 'B':
            return 34;
        case 'C':
            return 35;
        case 'D':
            return 36;
        case 'E':
            return 37;
        case 'F':
            return 38;
        case 'G':
            return 39;
        case 'H':
            return 40;
        case 'I':
            return 41;
        case 'J':
            return 42;
        case 'K':
            return 43;
        case 'L':
            return 44;
        case 'M':
            return 45;
        case 'N':
            return 46;
        case 'O':
            return 47;
        case 'P':
            return 48;
        case 'Q':
            return 49;
        case 'R':
            return 50;
        case 'S':
            return 51;
        case 'T':
            return 52;
        case 'U':
            return 53;
        case 'V':
            return 54;
        case 'W':
            return 55;
        case 'X':
            return 56;
        case 'Y':
            return 57;
        case 'Z':
            return 58;
        default:
            return 0;
    }
}