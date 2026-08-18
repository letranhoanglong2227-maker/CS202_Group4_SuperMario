#include "Objects/Blocks/Block.hpp"

Block::Block() : animationComponent(entitySprite, TextureBlockManager::getBlocksTexture())
{
    position = {0, 0};
    exist = true;
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture());
}

void Block::setSizeBlock(const sf::Vector2f& sz) {
    GameObject::setSize(sz);
    if (entitySprite.getTextureRect().width > 0 && entitySprite.getTextureRect().height > 0) {
        float scaleX = sz.x / static_cast<float>(entitySprite.getTextureRect().width);
        float scaleY = sz.y / static_cast<float>(entitySprite.getTextureRect().height);
        entitySprite.setScale(scaleX, scaleY);
    }
}

Block::~Block(){}
