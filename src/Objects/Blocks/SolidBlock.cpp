#include "Objects/Blocks/SolidBlock.hpp"

SolidBlock::SolidBlock()
{
    this->name = "SolidBlock";
    this->size = sf::Vector2f(32.f, 32.f);
    this->hitbox.setSize(this->size);
    this->initSpritesSheet();
}

SolidBlock::SolidBlock(sf::Vector2f pos, sf::Vector2f size)
{
    this->name = "SolidBlock";
    this->size = size;
    this->setPosition(pos);
    this->hitbox.setSize(this->size);
    this->initSpritesSheet();
}

SolidBlock::SolidBlock(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect, sf::Vector2f size)
{
    this->name = "SolidBlock";
    this->size = size;
    this->entityTexture = texture;
    this->entitySprite.setTexture(this->entityTexture);
    this->entitySprite.setTextureRect(textureRect);
    this->setPosition(pos);
    this->hitbox.setSize(this->size);
    this->initSpritesSheet();
}

void SolidBlock::initSpritesSheet()
{
    sf::IntRect defaultRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32));
    this->spriteSheets["IDLE"] = defaultRect;
}

void SolidBlock::reactToCollison(int collidedSide)
{
    (void)collidedSide;
}
