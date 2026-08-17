#pragma once

#include "Objects/Blocks/Block.hpp"

class Trampoline : public Block {
public:
    explicit Trampoline(sf::Vector2f position = {}, float launchVelocity = -700.f);
    void initSpritesSheet() override;
    void reactToCollision(int collidedSide) override;
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    float getLaunchVelocity() const noexcept;
private:
    sf::RectangleShape shape;
    float launchVelocity;
    float compressionTimer{0.f};
};

