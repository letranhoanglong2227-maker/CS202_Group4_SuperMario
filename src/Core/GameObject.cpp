#include "Core/GameObject.hpp"

GameObject::GameObject()
    : position(0.f, 0.f), size(0.f, 0.f), entityTexture(), entitySprite(entityTexture), eventMediator(nullptr)
{
}

sf::Vector2f GameObject::getPosition() const
{
    return this->position;
}

sf::Vector2f GameObject::getSize() const
{
    return this->size;
}

sf::Vector2f GameObject::getCenter() const
{
    return sf::Vector2f(this->position.x + this->size.x / 2.f, this->position.y + this->size.y / 2.f);
}

void GameObject::setPosition(const sf::Vector2f& pos)
{
    this->position = pos;
    this->entitySprite.setPosition(pos);
    this->hitbox.setPosition(pos);
}

void GameObject::setEventMediator(GameEventMediator* mediator)
{
    this->eventMediator = mediator;
}
