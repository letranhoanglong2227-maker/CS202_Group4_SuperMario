#include "Objects/Environment/Bullet.hpp"
#include "Levels/Managers/MapManager.hpp"

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f initialVelocity)
    : velocity(initialVelocity), shape(MapFormat::TILE_SIZE * 0.25f) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE * 0.5f, MapFormat::TILE_SIZE * 0.5f});
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(40, 40, 40));
}

void Bullet::update(float dt) {
    if (!active || dt <= 0.f) return;
    setPosition(getPosition() + velocity * dt);
    shape.setPosition(getPosition());
    lifetime -= dt;
    if (lifetime <= 0.f) active = false;
}

void Bullet::render(sf::RenderTarget* target) {
    if (target && active) target->draw(shape);
}

bool Bullet::isActive() const noexcept { return active; }
void Bullet::deactivate() noexcept { active = false; }

