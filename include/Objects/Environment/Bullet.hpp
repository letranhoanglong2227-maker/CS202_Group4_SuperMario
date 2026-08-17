#pragma once

#include "Core/GameObject.hpp"

class Bullet : public GameObject {
public:
    Bullet(sf::Vector2f position = {}, sf::Vector2f velocity = {-180.f, 0.f});
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    bool isActive() const noexcept;
    void deactivate() noexcept;
private:
    sf::Vector2f velocity;
    sf::CircleShape shape;
    float lifetime{8.f};
    bool active{true};
};

