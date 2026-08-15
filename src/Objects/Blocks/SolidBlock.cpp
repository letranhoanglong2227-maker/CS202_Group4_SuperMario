#include "Objects/Blocks/SolidBlock.hpp"

SolidBlock::SolidBlock(){
    name = "SolidBlock";
    initSpritesSheet();
    animationComponent.addAnimation("Idle", {spritesSheet["SolidBlock"]});
}

void SolidBlock::initSpritesSheet()
{
    spritesSheet["SolidBlock"] = sf::IntRect({2, 113}, {16, 16});
}

void SolidBlock::reactToCollision(int collidedSide){
    // Nothing to do (or maybe not yet)
}

void SolidBlock::update(float dt){
    animationComponent.play("Idle", dt);
}

SolidBlock::~SolidBlock(){}