#ifndef ZWEI_HUD_H
#define ZWEI_HUD_H

#include "Component.h"

class Hud : public Component {
public:
    Hud(Entity &parent);

    void render() override;

private:

    void fastNums(int num, int *h, int *t, int *o);

    void renderNumber(SDL_Rect &target, int num, int h, int t, int o);

    std::shared_ptr<Texture> texture;
};


#endif //ZWEI_HUD_H
