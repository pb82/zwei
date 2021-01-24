#ifndef ZWEI_ANIMATION_H
#define ZWEI_ANIMATION_H

#include <unordered_map>
#include <vector>
#include <queue>

#include "Component.h"
#include "../alg/Direction.h"

struct MixinFrame {
    std::unordered_map<Direction, int> frames;
    float duration = 0.0f;
};

class Animation : public Component {
public:
    Animation(Entity &parent, float speed, bool repeat);

    void update(float dt) override;

    void addAnimationFrame(int a);

    void addAnimationFrame(int n, int s, int e, int w);

    void addMixinFrame(int a);

    void addMixinFrame(int n, int s, int e, int w);

    void addAttackFrame(int n, int s, int e, int w, float duration, bool projectile = false);

    void queueAttackFrames();

    void queueProjectileFrames();

    int getCurrentFrame(Direction d = N);

    void start();

    void stop();

    float speed;

    bool repeat;

    float rotate = 0.0f;

private:

    bool paused = false;

    void addFrameForDirection(Direction d, int frame);

    float animationCount = 0;

    int currentIndex = 0;

    int frameCount = 0;

    std::unordered_map<Direction, std::vector<int>> frames;

    std::queue<MixinFrame> mixinFrames;

    std::vector<MixinFrame> attackFrames;

    std::vector<MixinFrame> projectileAttackFrames;

};


#endif
