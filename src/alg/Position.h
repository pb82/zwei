#ifndef ZWEI_POSITION_H
#define ZWEI_POSITION_H

#include <cmath>
#include <limits>

class Position {
public:
    Position() : x(0.0f), y(0.0f), p(0.0f) {}

    Position(float x, float y) : x(x), y(y), p(0.0f) {}

    Position(float x, float y, float p) : x(x), y(y), p(p) {}

    Position(const Position &other) noexcept {
        this->x = other.x;
        this->y = other.y;
        this->p = other.p;
    }

    Position(const Position &&other) noexcept
            : x(other.x),
              y(other.y),
              p(other.p) {}

    Position &operator=(const Position &other) {
        this->x = other.x;
        this->y = other.y;
        this->p = other.p;
        return *this;
    }

    bool operator==(const Position &other) {
        auto epsilon = std::numeric_limits<float>::epsilon();
        return closeTo(other, epsilon);
    }

    bool closeTo(const Position &other, float epsilon) {
        return
                std::abs(x - other.x) < epsilon &&
                std::abs(y - other.y) < epsilon;
    }

    float distance(const Position &other) const noexcept {
        return std::sqrt(
                std::pow(other.x - x, 2) +
                std::pow(other.y - y, 2));
    }

    void nearestTile(Position &other) {
        other.x = std::ceil(x);
        other.y = std::ceil(y);
    }

    float angle(const Position &other) {
        return std::atan2(y - other.y, other.x - x);
    }

    float x;
    float y;

    // Priority when used for path finding
    float p = 0;
};

#endif
