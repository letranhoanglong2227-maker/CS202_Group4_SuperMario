/* Note:
    Nhân: Đây là code tui viết tạm thời để test lại object block
*/

// include/Core/GameObject.hpp
#pragma once
#include<iostream>
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
    GameObject();
    sf::RectangleShape hitbox;

    virtual ~GameObject() = default;

    // Getters & Setters
    virtual sf::Vector2f getPosition() const { return position; }
    virtual void setPosition(const sf::Vector2f& pos) { 
        position = pos; 
        entitySprite.setPosition(pos);
        hitbox.setPosition(pos);
    }
    
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget* target) {
        if (target) {
            target->draw(entitySprite);
        }
    }
};