#ifndef ZWEI_POSITION_H
#define ZWEI_POSITION_H

#include <cmath>
#include <limits>
#include <vector>

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
        float cx = std::ceil(x);
        float cy = std::ceil(y);

        if ((cx - x) <= 0.5f) {
            other.x = cx;
        } else {
            other.x = std::floor(x);
        }

        if ((cy - y) <= 0.5f) {
            other.y = cy;
        } else {
            other.y = std::floor(y);
        }
    }

    float angle(const Position &other) {
        return std::atan2(y - other.y, other.x - x);
    }

    void bresenham(Position other, std::vector<Position> &points) {
        int x1 = this->x;
        int y1 = this->y;
        int x2 = other.x;
        int y2 = other.y;

        const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
        if (steep) {
            std::swap(x1, y1);
            std::swap(x2, y2);
        }

        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        const float dx = x2 - x1;
        const float dy = fabs(y2 - y1);

        float error = dx / 2.0f;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = (int) y1;

        const int maxX = (int) x2;

        for (int x = (int) x1; x <= maxX; x++) {
            if (steep) {
                points.emplace_back((float) y, (float) x, 0);
            } else {
                points.emplace_back((float) x, (float) y, 0);
            }

            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
        }
    }

    float x;
    float y;

    // Priority when used for path finding
    float p = 0;
};

#endif
