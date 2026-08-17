#include "Objects/Environment/Trampoline.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>

Trampoline::Trampoline(sf::Vector2f pos, float launch)
    : launchVelocity(launch) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE});
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(220, 45, 65));
    initSpritesSheet();
}

void Trampoline::initSpritesSheet() {
    spritesSheet["Idle"] = sf::IntRect({0, 0}, {16, 16});
}

void Trampoline::reactToCollision(int side) {
    if (side == 1) compressionTimer = 0.12f;
}

void Trampoline::update(float dt) {
    compressionTimer = std::max(0.f, compressionTimer - std::max(0.f, dt));
    shape.setSize({getSize().x,
                   compressionTimer > 0.f ? getSize().y * 0.65f : getSize().y});
    shape.setPosition({getPosition().x,
                       getPosition().y + getSize().y - shape.getSize().y});
}

void Trampoline::render(sf::RenderTarget* target) {
    if (target) target->draw(shape);
}

float Trampoline::getLaunchVelocity() const noexcept { return launchVelocity; }

