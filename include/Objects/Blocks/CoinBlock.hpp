#pragma once
#include "Objects/Blocks/Block.hpp"
#include "Core/Constants.hpp"
#include <functional>

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
