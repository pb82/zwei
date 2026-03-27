#include "Torch.h"

#include "../Manager.h"
#include "../Transform.h"
#include "../../Lighting.h"

Torch::Torch() : Item(TORCH) {}

bool Torch::stackable() {
    return true;
}

bool Torch::notification(std::string &s) {
    s = "Torch collected";
    return true;
}

int Torch::tile() {
    return 108;
}

bool Torch::use(std::shared_ptr<Entity> on) {
    if (RT_Lighting.hasActiveTorch()) return false;
    auto t = on->getComponent<Transform>();
    Manager::instance().addTimer(tile(), lifetime);
    RT_Lighting.addDynamic(&t->p, 512.0f, lifetime, tile());
    return true;
}