#include "Objects/Environment/Lava.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Objects/Blocks/Block.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

Lava::Lava(sf::Vector2f pos) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE});
    shape.setPosition(pos);
    shape.setSize(getSize());
    entitySprite.setTexture(TextureBlockManager::getBlocksTexture(), true);
    entitySprite.setTextureRect({{273, 154}, {16, 16}});
    entitySprite.setScale({MapFormat::TILE_SIZE / 16.f,
                           MapFormat::TILE_SIZE / 16.f});
    entitySprite.setPosition(pos);
}

void Lava::update(float dt) {
    animationTime += std::max(0.f, dt);
    const auto green = static_cast<std::uint8_t>(
        30.f + 30.f * (0.5f + 0.5f * std::sin(animationTime * 5.f)));
    shape.setFillColor(sf::Color(255, green, 20));
    constexpr std::array frames{
        sf::Vector2i{273, 154}, sf::Vector2i{290, 154},
        sf::Vector2i{307, 154}, sf::Vector2i{324, 154},
        sf::Vector2i{273, 171}, sf::Vector2i{290, 171},
        sf::Vector2i{307, 171}, sf::Vector2i{324, 171}};
    constexpr std::size_t pingPongLength = 2 * frames.size() - 2;
    const std::size_t phase =
        static_cast<std::size_t>(animationTime / 0.12f) % pingPongLength;
    animationFrame = phase < frames.size() ? phase : pingPongLength - phase;
    entitySprite.setTextureRect({frames[animationFrame], {16, 16}});
    entitySprite.setPosition(getPosition());
}

void Lava::render(sf::RenderTarget* target) {
    if (target) target->draw(entitySprite);
}

bool Lava::applyTo(PlayerManager& player) const {
    if (player.isDead()) return false;
    player.takeDamage(1);
    return player.isDead();
}

