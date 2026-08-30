#include "Core/GameObject.hpp"

GameObject::GameObject() : entitySprite(entityTexture) {

}

sf::Vector2f GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const sf::Vector2f& pos) {
    position = pos;
    entitySprite.setPosition(pos);
    hitbox.setPosition(pos);
}

void GameObject::setPosition(float x, float y) {
    setPosition(sf::Vector2f(x, y));
}

sf::Vector2f GameObject::getCenter() const {
    return sf::Vector2f(position.x + size.x / 2.f, position.y + size.y / 2.f);
}

sf::Vector2f GameObject::getSize() const {
    return size;
}

void GameObject::setSize(const sf::Vector2f& sz) {
    size = sz;
    hitbox.setSize(sz);
}

void GameObject::setSize(float width, float height) {
    setSize(sf::Vector2f(width, height));
}

void GameObject::setEventMediator(GameEventMediator* mediator) {
    eventMediator = mediator;
}

void GameObject::render(sf::RenderTarget* target) {
    if (target) {
        target->draw(entitySprite);
    }
}
