#ifndef ZWEI_POSITION_H
#define ZWEI_POSITION_H

class Position {
public:
    Position(float x, float y) : x(x), y(y) {}

    Position(const Position &other) noexcept {
        this->x = other.x;
        this->y = other.y;
    }

    Position(const Position &&other) noexcept: x(other.x), y(other.y) {}

    Position &operator=(const Position &other) {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }

    float x;
    float y;
};

#endif
