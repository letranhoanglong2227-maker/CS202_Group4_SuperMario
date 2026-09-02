#pragma once
#include "Objects/Blocks/Block.hpp"
#include "Core/Constants.hpp"
#include <functional>
#include <memory>

class MushroomBlock : public Block {
private:
    bool isEmpty;
    float originalY;
    
    // Bounce effect variables
    bool isBouncing;
    float bounceTimer;
    float bounceDuration;
    float maxBounceHeight;

    using SpawnCallback = std::function<void(std::unique_ptr<GameObject>)>;
    SpawnCallback onSpawnItem;
    int type;

public:
    MushroomBlock(SpawnCallback spawnCallback = {},
                  int mushroomType = 2);

    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;
    bool isBumpingUpward() const noexcept override;

    ~MushroomBlock() = default;
};
