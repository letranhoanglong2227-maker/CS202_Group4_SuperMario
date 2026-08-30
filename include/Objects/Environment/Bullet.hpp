#pragma once

#include "Core/GameObject.hpp"

class Bullet : public GameObject {
public:
    Bullet(sf::Vector2f position = {}, sf::Vector2f velocity = {-180.f, 0.f},
           float lifetime = 8.f);
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    bool isActive() const noexcept;
    void deactivate() noexcept;
    bool deactivateOnWorldCollision(const sf::FloatRect& obstacle) noexcept;
    bool cullOutside(const sf::FloatRect& worldBounds,
                     float margin = 0.f) noexcept;
    sf::FloatRect getPreviousBounds() const noexcept;
private:
    sf::Vector2f velocity;
    sf::FloatRect previousBounds;
    sf::CircleShape shape;
    float lifetime;
    bool active{true};
};

