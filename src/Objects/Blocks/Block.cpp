#include "Objects/Blocks/Block.hpp"

Block::Block()
    : GameObject(), name("Block"), exist(true)
{
}

bool Block::exists() const
{
    return this->exist;
}

void Block::setExist(bool value)
{
    this->exist = value;
}

const std::string& Block::getName() const
{
    return this->name;
}

void Block::update(float dt)
{
    if (!this->exist) return;
    if (this->animationComponent) {
        this->animationComponent->play("IDLE", dt);
    }
}

void Block::render(sf::RenderTarget* target)
{
    if (!this->exist || !target) return;
    target->draw(this->entitySprite);
}
