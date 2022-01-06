#include "Torch.h"

#include "../Manager.h"
#include "../Stats.h"
#include "../inv/TorchModifier.h"

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
    auto stats = on->getComponent<Stats>();
    if (!stats->inventory.hasModifier(CIRCLE_OF_LIGHT)) {
        Manager::instance().addTimer(tile(), lifetime);
        stats->inventory.addModifier<TorchModifier>(this->lifetime, tile());
        return true;
    }
    return false;
}