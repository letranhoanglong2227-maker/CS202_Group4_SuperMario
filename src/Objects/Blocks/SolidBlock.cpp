#include "Objects/Blocks/SolidBlock.hpp"

SolidBlock::SolidBlock(){
    name = "SolidBlock";
    initSpritesSheet();
}

SolidBlock::SolidBlock(float x, float y) : Block(x, y){
    name = "SolidBlock";
    initSpritesSheet();
}

void SolidBlock::initSpritesSheet()
{
    spritesSheet["SolidBlock"] = sf::IntRect({2, 113}, {16, 16});
}

void SolidBlock::reacToCollison(int collidedSide){
    // Nothing to do (or maybe not yet)
}

void SolidBlock::update(float dt){
    // Nothing to do (quite sure)
}

void SolidBlock::setupTexture(){
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture());
    entitySprite.setTextureRect(spritesSheet["SolidBlock"]);
}

SolidBlock::~SolidBlock(){}