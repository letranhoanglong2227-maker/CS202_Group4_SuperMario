#include "Objects/Blocks/MovingBlock.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <cmath>

MovingBlock::MovingBlock(sf::Vector2f pos, int widthInTiles,
                         sf::Vector2f distance, float moveSpeed)
    : origin(pos), travel(distance), speed(std::max(0.f, moveSpeed)) {
    setPosition(pos);
    setSize({static_cast<float>(std::max(1, widthInTiles)) * MapFormat::TILE_SIZE,
             MapFormat::TILE_SIZE});
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(238, 238, 238));
}

void MovingBlock::update(float dt) {
    const sf::Vector2f previous = getPosition();
    const float distance = std::sqrt(travel.x * travel.x + travel.y * travel.y);
    if (distance > 0.f && dt > 0.f) {
        progress += direction * speed * dt / distance;
        if (progress >= 1.f) { progress = 1.f; direction = -1.f; }
        if (progress <= 0.f) { progress = 0.f; direction = 1.f; }
        setPosition(origin + travel * progress);
    }
    frameDelta = getPosition() - previous;
    shape.setPosition(getPosition());
}

void MovingBlock::render(sf::RenderTarget* target) {
    if (target) target->draw(shape);
}

sf::Vector2f MovingBlock::getFrameDelta() const noexcept { return frameDelta; }

