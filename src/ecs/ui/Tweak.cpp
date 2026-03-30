#include "Tweak.h"

Tweak::Tweak(Entity &parent, std::shared_ptr<Entity> client)
        : Component(parent), client(client) {}

void Tweak::render(uint8_t) {}
