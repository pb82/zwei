#ifndef ZWEI_MIND_H
#define ZWEI_MIND_H

#include "../Entity.h"
#include "../Collider.h"

class Mind {
public:
    Mind(Entity &parent) : parent(parent) {}

    virtual ~Mind() {}

    virtual void plan(float dt) {}

    virtual int delay() { return 1000; }

    virtual bool activate() { return false; }

    virtual void render() { return; }

protected:

    Entity &parent;

    float getDistanceFromPlayer();

    float getDistanceFrom(std::shared_ptr<Entity> entity);

    float getAngleTo(std::shared_ptr<Entity> entity);

    bool hasDirectPathToPlayer();

    bool hasDirectPathTo(std::shared_ptr<Entity> entity);

    bool isInRangeForAttack(std::shared_ptr<Entity> entity);

    bool isInRangeForAttack(std::shared_ptr<Entity> entity, float range);

    bool hasRouteToPlayer(std::vector<Position> &route);

    bool hasRouteTo(std::vector<Position> &route, std::shared_ptr<Entity> entity);

    bool enemiesInRange(float distance, std::vector<std::shared_ptr<Entity>> &enemies);

    bool alliesInRange(float distance, std::vector<std::shared_ptr<Entity>> &enemies);

    void followRoute(std::vector<Position> &route);

    void stop();

    void attack();

    void turn(float angle);

    void walkTowardsPlayer();

    void walkTowards(std::shared_ptr<Entity> entity);


private:


};

#endif
