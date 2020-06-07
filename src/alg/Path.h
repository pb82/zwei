#ifndef ZWEI_PATH_H
#define ZWEI_PATH_H

#include <vector>
#include <memory>
#include <queue>
#include <map>

#include "Position.h"

class Topology {
public:

    Topology(int w, int h);

    bool accessible(int x, int y) const;

    void neighbours(int x, int y, std::vector<Position> &n);

    std::vector<bool> topology;

    int index(int x, int y) const;

    int index(const Position &p) const;

private:

    int w;
    int h;
};

class PositionCompare {
public:
    bool operator()(const Position &a, const Position &b) {
        return a.p > b.p;
    }
};

class Path {
public:
    Path(std::shared_ptr<Topology> topology);

    bool calculate(const Position &start, const Position &goal, std::vector<Position> &path);

private:
    void replay(std::vector<Position> &path, const Position &start, const Position &goal);

    std::shared_ptr<Topology> topology;

    std::priority_queue<Position, std::vector<Position>, PositionCompare> frontier;

    std::map<int, Position> cameFrom;

};


#endif
