#include "Item.h"
#include "../Collectable.h"
#include "../Transform.h"
#include "../Collider.h"
#include "HealthPotion.h"
#include "StickItem.h"
#include "BowItem.h"
#include "../Analytics.h"
#include "Torch.h"

std::shared_ptr<Item> Item::make(ItemType type) {
    switch (type) {
        case EMPTY_SLOT:
            return nullptr;
        case HEALTH_POTION:
            return std::make_shared<HealthPotion>();
        case STICK:
            return std::make_shared<StickItem>();
        case BOW:
            return std::make_shared<BowItem>();
        case TORCH:
            return std::make_shared<Torch>();
        default:
            return nullptr;
    }
}

std::shared_ptr<Entity> Item::make(Position p, ItemType type) {
    std::shared_ptr<Item> item = make(type);
    return make(p, item);
}

std::shared_ptr<Entity> Item::make(Position p, std::shared_ptr<Item> item) {
    auto entity = std::make_shared<Entity>();

    entity->addComponent<Transform>(p.x, p.y, Padding{0.5, 0.5, 0.5, 0.5});
    auto transform = entity->getComponent<Transform>();

    entity->addComponent<Collider>(transform, CT_ITEM, Padding{0.7, 0.7, 0.7, 0.7});
    entity->addComponent<Collectable>(item);
    entity->addComponent<Analytics>();

    return entity;
}