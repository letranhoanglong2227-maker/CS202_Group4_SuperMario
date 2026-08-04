#include "Objects/Blocks/Block.hpp"

Block::Block()
{
    position = {0, 0};
    exist = false;
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture());
}

Block::Block(float x, float y)
{
    position = {x, y};
    exist = false;
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture());
}

Block::~Block(){}
