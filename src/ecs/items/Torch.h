#ifndef ZWEI_TORCH_H
#define ZWEI_TORCH_H

#include "Item.h"

class Torch : public Item {
public:
    Torch();

    bool stackable() override;

    int tile() override;

    bool notification(std::string &s) override;

    bool use(std::shared_ptr<Entity> on) override;

private:

    float lifetime = 60000;

};


#endif //ZWEI_TORCH_H
