#include "Objects/Blocks/Block.hpp"

Block::Block() : animationComponent(entitySprite, TextureBlockManager::getBlocksTexture())
{
    position = {0, 0};
    exist = false;
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture());
}

Block::~Block(){}
