#include "Objects/Blocks/Block.hpp"

Block::Block() : animationComponent(entitySprite, TextureBlockManager::getBlocksTexture())
{
    position = {0, 0};
    exist = true;
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture());
}

void Block::setSizeBlock(const sf::Vector2f& sz) {
    GameObject::setSize(sz);
    sf::Vector2i frameSize = entitySprite.getTextureRect().size;
    if (frameSize.x <= 0 || frameSize.y <= 0) frameSize = {16, 16};
    entitySprite.setScale(
        {sz.x / static_cast<float>(frameSize.x),
         sz.y / static_cast<float>(frameSize.y)});
}

Block::~Block(){}
