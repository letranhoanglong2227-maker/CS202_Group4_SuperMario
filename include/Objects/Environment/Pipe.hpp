#pragma once

#include "Objects/Blocks/SolidBlock.hpp"

class Pipe : public SolidBlock {
public:
    Pipe(sf::Vector2f position = {}, int heightInTiles = 2);
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
private:
    sf::RectangleShape body;
};

