#pragma once
#include"Block.hpp"

class SolidBlock : public Block{
public:
    SolidBlock();

    void initSpritesSheet();
    void reactToCollision(int collidedSide);
    void update(float dt);

    ~SolidBlock();
};

