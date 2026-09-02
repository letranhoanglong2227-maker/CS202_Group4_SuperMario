#include "Objects/Blocks/SolidBlock.hpp"

SolidBlock::SolidBlock(sf::IntRect selectedFrame) : frame(selectedFrame) {
    name = "SolidBlock";
    exist = true;
    initSpritesSheet();
    animationComponent.addAnimation("Idle", {spritesSheet["SolidBlock"]});
}

void SolidBlock::initSpritesSheet()
{
    spritesSheet["SolidBlock"] = frame;
}

void SolidBlock::reactToCollision(int collidedSide){
    (void)collidedSide;
    // Nothing to do (or maybe not yet)
}

void SolidBlock::update(float dt){
    animationComponent.play("Idle", dt);
}

SolidBlock::~SolidBlock(){}
