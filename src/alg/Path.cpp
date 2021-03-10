#include <algorithm>

#include "Path.h"


Topology::Topology() {}

void Topology::make(int w, int h) {
    this->w = w;
    this->h = h;
}

void Topology::clear() {
    topology.clear();
    mobiles.clear();
}

std::vector<bool> &Topology::data() {
    return this->topology;
}

int Topology::index(int x, int y) const {
    return (y * w) + x;
}

int Topology::index(const Position &p) const {
    return index(p.x, p.y);
}

void Topology::registerMobile(Position *position) {
    this->mobiles.push_back(position);
}

void Topology::unregisterMobile(Position *position) {
    mobiles.erase(std::remove_if(this->mobiles.begin(), this->mobiles.end(), [&](Position *p) {
        return p == position;
    }), mobiles.end());
}

bool Topology::accessible(int x, int y) const {
    if (x < 0) return false;
    if (y < 0) return false;
    int i = index(x, y);
    if (i < 0) return false;
    if (i >= topology.size()) return false;

    // Is an enemy or other mobile object at this position?
    for (auto ptr : this->mobiles) {
        int _x = std::ceil(ptr->x);
        int _y = std::ceil(ptr->y);
        if (_x == x && _y == y) return false;
    }
    return topology.at(i) == 0;
}

bool Topology::allAccessible(std::vector<Position> &line, Position &own) {
    for (auto &segment : line) {
        if (segment == own) continue;
        if (!accessible(segment.x, segment.y)) {
            return false;
        }
    }
    return true;
}

Position Topology::nearestAccessible(Position &p, bool includeOwn) {
    if (includeOwn && accessible(p.x, p.y)) {
        return p;
    }

    std::vector<Position> n;
    neighbours(p.x, p.y, n);

    // Get next accessible neighbour
    if (!n.empty()) return n.at(0);

    // None accessible
    return p;
}

void Topology::neighbours(int x, int y, std::vector<Position> &n) const {
    if (accessible(x - 1, y)) n.emplace_back((float) x - 1, (float) y);
    if (accessible(x + 1, y)) n.emplace_back((float) x + 1, (float) y);
    if (accessible(x, y - 1)) n.emplace_back((float) x, (float) y - 1);
    if (accessible(x, y + 1)) n.emplace_back((float) x, (float) y + 1);
}

Path::Path(const Topology &t) : topology(t) {}

void Path::replay(std::vector<Position> &path, const Position &start, const Position &goal) {
    auto current = goal;
    path.push_back(current);

    int length = cameFrom.size();

    while (length--) {
        current = cameFrom[topology.index(current)];

        if (current == start) {
            break;
        }
        path.push_back(current);
    }
}

bool Path::calculate(const Position &start, const Position &goal, std::vector<Position> &path) {
    bool finished = false;
    frontier.push(start);

    std::map<int, float> costSoFar;
    costSoFar[topology.index(start)] = 0;

    while (!frontier.empty() && !finished) {
        auto current = frontier.top();
        frontier.pop();

        if (current == goal) {
            finished = true;
            break;
        }

        int currentIndex = topology.index(current);

        std::vector<Position> neighbours;
        topology.neighbours(current.x, current.y, neighbours);

        for (auto next: neighbours) {
            int nextIndex = topology.index(next);
            int newCost = costSoFar[currentIndex] + 1;

            if (costSoFar.find(nextIndex) == costSoFar.end() || newCost < costSoFar[nextIndex]) {
                costSoFar[nextIndex] = newCost;
                next.p = newCost + next.distance(goal);
                frontier.push(next);
                cameFrom[nextIndex] = current;
            }
        }
    }

    if (finished) {
        replay(path, start, goal);
    }

    return finished;
}