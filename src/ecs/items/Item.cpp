#include "Item.h"
#include "../Collectable.h"
#include "../Transform.h"
#include "../Collider.h"
#include "HealthPotion.h"
#include "StickItem.h"
#include "BowItem.h"
#include "../Analytics.h"

std::shared_ptr<Entity> Item::make(Position p, ItemType type) {
    std::shared_ptr<Item> item = nullptr;
    switch (type) {
        case EMPTY_SLOT:
            return nullptr;
        case HEALTH_POTION:
            item = std::make_shared<HealthPotion>();
            break;
        case STICK:
            item = std::make_shared<StickItem>();
            break;
        case BOW:
            item = std::make_shared<BowItem>();
            break;
        default:
            return nullptr;
    }

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