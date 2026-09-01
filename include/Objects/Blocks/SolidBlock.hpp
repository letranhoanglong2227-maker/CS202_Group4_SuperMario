#pragma once
#include"Block.hpp"

class SolidBlock : public Block{
public:
    explicit SolidBlock(sf::IntRect frame = {{18, 1}, {16, 16}});

    void initSpritesSheet();
    void reactToCollision(int collidedSide);
    void update(float dt);

    ~SolidBlock();

private:
    sf::IntRect frame;
};

