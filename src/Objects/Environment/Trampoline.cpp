#include "Objects/Environment/Trampoline.hpp"
#include "Core/Constants.hpp"
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
    animationComponent.addAnimation("Idle", {spritesSheet["Idle"]});
    animationComponent.addAnimation("Compressed", {spritesSheet["Compressed"]});
    setSizeBlock(getSize());
    entitySprite.setPosition(pos);
}

void Trampoline::initSpritesSheet() {
    spritesSheet["Idle"] = sf::IntRect({18, 232}, {16, 24});
    spritesSheet["Compressed"] = sf::IntRect({35, 232}, {16, 24});
}

void Trampoline::reactToCollision(int side) {
    if (side == COLLISION_TOP && compressionTimer <= 0.f) {
        compressionTimer = 0.12f;
    }
}

void Trampoline::update(float dt) {
    compressionTimer = std::max(0.f, compressionTimer - std::max(0.f, dt));
    animationComponent.play(
        compressionTimer > 0.f ? "Compressed" : "Idle", dt);
    shape.setSize({getSize().x,
                   compressionTimer > 0.f ? getSize().y * 0.65f : getSize().y});
    shape.setPosition({getPosition().x,
                       getPosition().y + getSize().y - shape.getSize().y});
    entitySprite.setPosition(getPosition());
}

void Trampoline::render(sf::RenderTarget* target) {
    if (target) target->draw(entitySprite);
}

float Trampoline::getLaunchVelocity() const noexcept { return launchVelocity; }

