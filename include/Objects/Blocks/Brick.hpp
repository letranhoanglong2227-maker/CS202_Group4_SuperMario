#pragma once
#include "Objects/Blocks/Block.hpp"
#include "Core/Constants.hpp"
#include <functional>
#include <memory>

class Brick : public Block {
private:
    bool isBroken;
    bool breakable;
    sf::IntRect frame;
    using SpawnCallback = std::function<void(std::unique_ptr<GameObject>)>;
    SpawnCallback onSpawnItem;

public:
    Brick(SpawnCallback spawnCallback = {},
          bool canBreak = true,
          sf::IntRect selectedFrame = {{1, 86}, {16, 16}});

    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;

    ~Brick() = default;
};
