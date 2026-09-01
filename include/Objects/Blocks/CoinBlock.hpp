#pragma once
#include "Objects/Blocks/Block.hpp"
#include "Core/Constants.hpp"
#include <functional>
#include <memory>

class CoinBlock : public Block {
private:
    int coinCount;
    float originalY;
    
    // Bounce effect variables
    bool isBouncing;
    float bounceTimer;
    float bounceDuration;
    float maxBounceHeight;

    using SpawnCallback = std::function<void(std::unique_ptr<GameObject>)>;
    using RewardCallback = std::function<void(int, int)>;
    SpawnCallback onSpawnItem;
    RewardCallback onReward;

public:
    CoinBlock(int count = 1, SpawnCallback spawnCallback = {},
              RewardCallback rewardCallback = {});

    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;
    bool isBumpingUpward() const noexcept override;

    ~CoinBlock() = default;
};
