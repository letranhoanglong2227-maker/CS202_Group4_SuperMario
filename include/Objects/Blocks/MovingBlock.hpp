#pragma once

#include "Objects/Blocks/SolidBlock.hpp"

class MovingBlock : public SolidBlock {
public:
    MovingBlock(sf::Vector2f position = {}, int widthInTiles = 2,
                sf::Vector2f travel = {96.f, 0.f}, float speed = 60.f);
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    sf::Vector2f getFrameDelta() const noexcept;
private:
    sf::Vector2f origin;
    sf::Vector2f travel;
    sf::Vector2f frameDelta{};
    sf::RectangleShape shape;
    float speed;
    float progress{0.f};
    float direction{1.f};
};

