#include "Group.h"

Group::Group(Entity &parent) : Component(parent) {}

void Group::addMember(std::shared_ptr<Entity> member) {
    this->members.push_back(member);
}

std::vector<std::shared_ptr<Entity>> &Group::all() {
    return this->members;
}