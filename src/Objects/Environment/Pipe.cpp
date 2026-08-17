#include "Objects/Environment/Pipe.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>

Pipe::Pipe(sf::Vector2f pos, int heightInTiles) {
    setPosition(pos);
    setSize({2.f * MapFormat::TILE_SIZE,
             static_cast<float>(std::max(1, heightInTiles)) * MapFormat::TILE_SIZE});
    body.setPosition(pos);
    body.setSize(getSize());
    body.setFillColor(sf::Color(38, 166, 91));
    body.setOutlineColor(sf::Color(18, 92, 48));
    body.setOutlineThickness(2.f);
}

void Pipe::update(float dt) {
    SolidBlock::update(dt);
    body.setPosition(getPosition());
}

void Pipe::render(sf::RenderTarget* target) {
    if (target) target->draw(body);
}

