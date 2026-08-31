#pragma once

#include "Objects/Blocks/SolidBlock.hpp"

#include <vector>

class Pipe : public SolidBlock {
public:
    Pipe(sf::Vector2f position = {}, int heightInTiles = 2,
         bool grayStyle = false);
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
private:
    sf::RectangleShape body;
    std::vector<sf::Sprite> tiles;
};

