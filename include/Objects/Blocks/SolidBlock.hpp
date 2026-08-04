#pragma once
#include"Block.hpp"

class SolidBlock : public Block{
public:
    SolidBlock();
    SolidBlock(float x, float y);

    void initSpritesSheet();
    void reacToCollison(int collidedSide);
    void update(float dt);

    void setupTexture();

    ~SolidBlock();
};

