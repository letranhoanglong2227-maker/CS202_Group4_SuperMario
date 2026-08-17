#include "Objects/Environment/Cannon.hpp"
#include "Objects/Environment/Bullet.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <utility>

Cannon::Cannon(sf::Vector2f pos, SpawnCallback callback,
               float interval, float fireDirection)
    : spawn(std::move(callback)), fireInterval(std::max(0.1f, interval)),
      direction(fireDirection < 0.f ? -1.f : 1.f) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE});
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(55, 55, 55));
    initSpritesSheet();
}

void Cannon::initSpritesSheet() {
    spritesSheet["Idle"] = sf::IntRect({0, 0}, {16, 16});
}

void Cannon::reactToCollision(int side) { (void)side; }

void Cannon::update(float dt) {
    if (dt <= 0.f) return;
    fireTimer += dt;
    if (spawn && fireTimer >= fireInterval) {
        fireTimer -= fireInterval;
        const sf::Vector2f bulletPosition{
            getPosition().x + (direction > 0.f ? getSize().x : -MapFormat::TILE_SIZE * 0.5f),
            getPosition().y + MapFormat::TILE_SIZE * 0.25f};
        spawn(std::make_unique<Bullet>(bulletPosition,
                                      sf::Vector2f{direction * 180.f, 0.f}));
    }
}

void Cannon::render(sf::RenderTarget* target) {
    if (target) target->draw(shape);
}

