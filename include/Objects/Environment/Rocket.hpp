#pragma once

#include "Core/GameObject.hpp"

class Rocket : public GameObject {
public:
    Rocket(sf::Vector2f position = {}, const GameObject* target = nullptr,
           float speed = 140.f);
    void setTarget(const GameObject* target) noexcept;
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
private:
    const GameObject* target;
    float speed;
    sf::Vector2f velocity{};
    sf::RectangleShape shape;
};

