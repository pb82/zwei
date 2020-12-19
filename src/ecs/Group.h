#ifndef ZWEI_GROUP_H
#define ZWEI_GROUP_H

#include <vector>
#include <memory>

#include "Component.h"

class Group : public Component {
public:
    Group(Entity &parent);

    void addMember(std::shared_ptr<Entity> member);

    std::vector<std::shared_ptr<Entity>> &all();

private:

    std::vector<std::shared_ptr<Entity>> members;

};


#endif //ZWEI_GROUP_H
