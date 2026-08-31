#include "Objects/Environment/Cannon.hpp"
#include "Objects/Environment/Rocket.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <utility>

Cannon::Cannon(sf::Vector2f pos, SpawnCallback callback, float interval)
    : spawn(std::move(callback)), fireInterval(std::max(0.1f, interval)) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE});
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(55, 55, 55));
    initSpritesSheet();
    animationComponent.addAnimation("Idle", {spritesSheet["Idle"]});
    setSizeBlock(getSize());
    entitySprite.setPosition(pos);
}

void Cannon::initSpritesSheet() {
    spritesSheet["Idle"] = sf::IntRect({69, 137}, {16, 16});
}

void Cannon::reactToCollision(int side) { (void)side; }

void Cannon::update(float dt) {
    if (dt <= 0.f) return;
    animationComponent.play("Idle", dt);
    fireTimer += dt;
    if (spawn && fireTimer >= fireInterval) {
        fireTimer -= fireInterval;
        const float y = getPosition().y - 1.f;
        spawn(std::make_unique<Rocket>(
            sf::Vector2f{getPosition().x - MapFormat::TILE_SIZE, y},
            sf::Vector2f{-500.f, 0.f}, 7.5f));
        spawn(std::make_unique<Rocket>(
            sf::Vector2f{getPosition().x + MapFormat::TILE_SIZE, y},
            sf::Vector2f{500.f, 0.f}, 7.5f));
    }
}

void Cannon::render(sf::RenderTarget* target) {
    if (target) target->draw(entitySprite);
}

