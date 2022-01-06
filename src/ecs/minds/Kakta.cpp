#include "../Transform.h"

#include "Kakta.h"
#include "../../Draw.h"
#include "../../alg/Rand.h"

Kakta::Kakta(Entity &parent) : Mind(parent) {}

Kakta::~Kakta() {}

int Kakta::delay() {
    return 200;
}

bool Kakta::activate() {
    float distance = getDistanceFromPlayer();
    if (distance < activationRange) {
        return true;
    }

    stop();
    return false;
}

void Kakta::plan(float dt) {
    auto player = RT_Context.getPlayer();

    if (isInRangeForAttack(player)) {
        turn(getAngleTo(player));
        attack();
        return;
    }

    if (alliesInRange(activationRange, trackedAllies)) {
        for (auto enemy: trackedAllies) {
            if (isInRangeForAttack(enemy)) {
                turn(getAngleTo(enemy));
                attack();
                return;
            }

            if (hasDirectPathTo(enemy)) {
                walkTowards(enemy);
                return;
            }
        }
    }

    if (hasDirectPathToPlayer()) {
        walkTowardsPlayer();
        return;
    }

    if (hasRouteToPlayer(route)) {
        followRoute(route);
        return;
    }

    if (route.size() == 1) {
        float direction = Rnd::getRandomFloat(VM_200_PI);
        Mind::backtrack(direction);
        return;
    }

    stop();
}

void Kakta::render() {
    /*
    if (hasDirectPathToPlayer()) {
        auto t1 = this->parent.getComponent<Transform>()->p;
        auto t2 = RT_Player->getComponent<Transform>()->p;

        SDL_Rect r1, r2;
        RT_Camera.project(r1, t1.x, t1.y);
        RT_Camera.project(r2, t2.x, t2.y);

        Draw::instance().line(color_Bad, r1.x, r1.y, r2.x, r2.y);
    } else if (!route.empty()) {
        for (auto p : route) {
            SDL_Rect r1;
            RT_Camera.project(r1, p.x, p.y);
            Draw::instance().box(color_Bad, r1);
        }
    }
    */
}