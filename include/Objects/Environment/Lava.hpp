#pragma once

#include "Core/GameObject.hpp"

class Lava : public GameObject {
public:
    explicit Lava(sf::Vector2f position = {});
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
private:
    sf::RectangleShape shape;
    float animationTime{0.f};
};

