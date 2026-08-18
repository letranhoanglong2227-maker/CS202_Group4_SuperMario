#pragma once
#include "Objects/Blocks/Block.hpp"
#include "Core/Constants.hpp"
#include <functional>

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
