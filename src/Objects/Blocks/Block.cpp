#include "Objects/Blocks/Block.hpp"

Block::Block() : animationComponent(entitySprite, TextureBlockManager::getBlocksTexture())
{
    position = {0, 0};
    exist = true;
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture());
}

void Block::setSizeBlock(const sf::Vector2f& sz) {
    GameObject::setSize(sz);
    if (entitySprite.getTextureRect().size.x > 0 && entitySprite.getTextureRect().size.y > 0) {
        float scaleX = sz.x / static_cast<float>(entitySprite.getTextureRect().size.x);
        float scaleY = sz.y / static_cast<float>(entitySprite.getTextureRect().size.y);
        entitySprite.setScale(sf::Vector2f(scaleX, scaleY));
    }
}

Block::~Block(){}
