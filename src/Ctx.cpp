#include "Ctx.h"

void Ctx::setPlayer(std::shared_ptr<Entity> player) {
    this->player = player;
}

std::shared_ptr<Entity> Ctx::getPlayer() {
    return this->player;
}