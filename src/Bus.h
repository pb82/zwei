#ifndef ZWEI_BUS_H
#define ZWEI_BUS_H

#include <functional>
#include <unordered_map>
#include <vector>

enum EventType {
    EventQuit,
    EventPlayerDamaged,
    EventPlayerDied,
    EventEnemyDied,
    EventItemCollected,
    EventSceneChange,
    EventStateChangeRequested,
};

struct Event {
    EventType type;
    explicit Event(EventType t) : type(t) {}
    virtual ~Event() = default;
};

struct EnemyDiedEvent : public Event {
    float x, y;
    EnemyDiedEvent(float x, float y) : Event(EventEnemyDied), x(x), y(y) {}
};

struct StateChangeRequestedEvent : public Event {
    int target;
    StateChangeRequestedEvent(int s) : Event(EventStateChangeRequested), target(s) {}
};

class Bus {
public:
    static Bus &instance() {
        static Bus instance;
        return instance;
    }

    Bus(Bus const &) = delete;
    void operator=(Bus const &) = delete;

    void subscribe(EventType type, std::function<void(const Event &)> cb);
    void publish(const Event &event);

private:
    Bus() = default;

    std::unordered_map<int, std::vector<std::function<void(const Event &)>>> subscribers;
};

#endif
