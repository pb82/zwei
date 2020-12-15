#include "Projectile.h"

Projectile::Projectile(Entity &parent) : Component(parent), force(0, 0), power(0), origin(nullptr) {}
