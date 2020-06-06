#ifndef ZWEI_POSITION_H
#define ZWEI_POSITION_H

#include <cmath>
#include <limits>

class Position {
public:
    Position() : x(0.0f), y(0.0f) {}

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

    bool operator==(const Position &other) {
        auto epsilon = std::numeric_limits<float>::epsilon();
        return
                std::abs(x - other.x) < epsilon &&
                std::abs(y - other.y) < epsilon;
    }

    float distance(const Position &other) {
        return std::sqrt(std::pow(other.x - x, 2) +
                         std::pow(other.y - y, 2));
    }

    float x;
    float y;

    // Priority when used for path finding
    float p = 0;
};

#endif
