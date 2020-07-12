#include "Path.h"

Topology::Topology() {}

void Topology::make(int w, int h) {
    this->w = w;
    this->h = h;
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

bool Topology::accessible(int x, int y) const {
    if (x < 0) return false;
    if (y < 0) return false;
    int i = index(x, y);
    if (i < 0) return false;
    if (i >= topology.size()) return false;
    return topology.at(i) == 0;
}

void Topology::neighbours(int x, int y, std::vector<Position> &n) {
    if (accessible(x - 1, y)) n.push_back({(float) x - 1, (float) y});
    if (accessible(x + 1, y)) n.push_back({(float) x + 1, (float) y});
    if (accessible(x, y - 1)) n.push_back({(float) x, (float) y - 1});
    if (accessible(x, y + 1)) n.push_back({(float) x, (float) y + 1});
}

Path::Path(std::shared_ptr<Topology> topology) : topology(topology) {}

void Path::replay(std::vector<Position> &path, const Position &start, const Position &goal) {
    auto current = goal;
    path.push_back(current);

    while (true) {
        current = cameFrom[topology->index(current)];

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
    costSoFar[topology->index(start)] = 0;

    while (!frontier.empty() && !finished) {
        auto current = frontier.top();
        frontier.pop();

        if (current == goal) {
            finished = true;
            break;
        }

        int currentIndex = topology->index(current);

        std::vector<Position> neighbours;
        topology->neighbours(current.x, current.y, neighbours);

        for (auto next: neighbours) {
            int nextIndex = topology->index(next);
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