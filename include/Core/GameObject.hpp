#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class GameEventMediator; // Forward declaration

class GameObject {
protected:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Sprite entitySprite;
    sf::Texture entityTexture;
    GameEventMediator* eventMediator{ nullptr };

public:
    sf::RectangleShape hitbox;

    GameObject();
    virtual ~GameObject() = default;

    // Getters & Setters Position
    virtual sf::Vector2f getPosition() const;
    virtual void setPosition(const sf::Vector2f& pos);
    virtual void setPosition(float x, float y);
    virtual sf::Vector2f getCenter() const;

    // Getters & Setters Size
    virtual sf::Vector2f getSize() const;
    virtual void setSize(const sf::Vector2f& sz);
    virtual void setSize(float width, float height);

    virtual void setEventMediator(GameEventMediator* mediator);

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget* target);
};