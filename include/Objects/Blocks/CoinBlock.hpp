#pragma once
#include "Objects/Blocks/Block.hpp"
#include <functional>

// You can coordinate with your partner to agree on these constants.
// For example: 0 = Bottom, 1 = Top, 2 = Left, 3 = Right
const int COLLISION_BOTTOM = 0; 

class CoinBlock : public Block {
private:
    int coinCount;
    float originalY;
    
    // Bounce effect variables
    bool isBouncing;
    float bounceVelocity;
    float bounceGravity;
    float maxBounceHeight;

    std::function<void(GameObject*)> onSpawnItem;

public:
    CoinBlock(int count = 1, std::function<void(GameObject*)> spawnCallback = nullptr);

    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;

    ~CoinBlock() = default;
};
