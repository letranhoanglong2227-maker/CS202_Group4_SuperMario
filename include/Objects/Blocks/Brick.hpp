#pragma once
#include "Objects/Blocks/Block.hpp"
#include <functional>

// Reusing the collision constant (usually defined elsewhere, e.g. CoinBlock or a common header)
// For now we'll extern it if it's already defined, or we can just define it if needed.
// Based on MushroomBlock, we can assume it's already defined or we define it in CoinBlock.hpp
extern const int COLLISION_BOTTOM;

class Brick : public Block {
private:
    bool isBroken;
    std::function<void(GameObject*)> onSpawnItem;

public:
    Brick(std::function<void(GameObject*)> spawnCallback = nullptr);

    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;

    ~Brick() = default;
};
