#pragma once
#include "Objects/Blocks/Block.hpp"

class BrickFragment : public Block {
private:
    sf::Vector2f velocity;
    float gravity;

public:
    BrickFragment(sf::Vector2f startPos, sf::Vector2f initialVelocity);

    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;

    ~BrickFragment() = default;
};
