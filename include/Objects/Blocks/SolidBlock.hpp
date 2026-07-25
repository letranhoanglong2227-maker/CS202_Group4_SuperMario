#pragma once

#include "Objects/Blocks/Block.hpp"

class SolidBlock : public Block {
public:
    SolidBlock();
    SolidBlock(sf::Vector2f pos, sf::Vector2f size = sf::Vector2f(32.f, 32.f));
    SolidBlock(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect, sf::Vector2f size = sf::Vector2f(32.f, 32.f));
    virtual ~SolidBlock() = default;

    void initSpritesSheet() override;
    void reactToCollison(int collidedSide) override;
};

