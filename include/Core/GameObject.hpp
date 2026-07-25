#pragma once

#include <SFML/Graphics.hpp>

class GameEventMediator;

class GameObject {
protected:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Texture entityTexture;
    sf::Sprite entitySprite;
    GameEventMediator* eventMediator{ nullptr };

public:
    sf::RectangleShape hitbox;

    GameObject();
    virtual ~GameObject() = default;

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::Vector2f getCenter() const;

    virtual void setPosition(const sf::Vector2f& pos);
    virtual void setEventMediator(GameEventMediator* mediator);

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget* target) = 0;
};

