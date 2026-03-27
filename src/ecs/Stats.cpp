#include "Stats.h"

#include "Entity.h"
#include "Transform.h"
#include "../Bus.h"

Stats::Stats(Entity &parent, bool render) : Component(parent), inventory(parent) {}

void Stats::update(float dt) {
    auto hp = this->character.getHitpoints();
    if (std::get<0>(hp) <= 0) {
        auto transform = this->parent.getComponent<Transform>();
        RT_Context.getTopology().unregisterMobile(&transform->p);
        this->parent.disable();
        Bus::instance().publish(EnemyDiedEvent(transform->p.x, transform->p.y));
    }
}
