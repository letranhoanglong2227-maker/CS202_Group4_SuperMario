#include "Objects/Environment/Lava.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>

Lava::Lava(sf::Vector2f pos) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE});
    shape.setPosition(pos);
    shape.setSize(getSize());
}

void Lava::update(float dt) {
    animationTime += std::max(0.f, dt);
    const auto green = static_cast<std::uint8_t>(
        30.f + 30.f * (0.5f + 0.5f * std::sin(animationTime * 5.f)));
    shape.setFillColor(sf::Color(255, green, 20));
}

void Lava::render(sf::RenderTarget* target) {
    if (target) target->draw(shape);
}

bool Lava::applyTo(PlayerManager& player) const {
    if (player.isDead()) return false;
    player.takeDamage(player.getHealth());
    return player.isDead();
}

