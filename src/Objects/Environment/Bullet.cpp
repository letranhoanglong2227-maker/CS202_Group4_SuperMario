#include "Objects/Environment/Bullet.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f initialVelocity,
               float activeLifetime)
    : velocity(initialVelocity), shape(MapFormat::TILE_SIZE * 0.25f),
      lifetime(std::max(0.f, activeLifetime)), active(activeLifetime > 0.f) {
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

bool Bullet::deactivateOnWorldCollision(
    const sf::FloatRect& obstacle) noexcept {
    if (!active || !hitbox.getGlobalBounds().findIntersection(obstacle)) {
        return false;
    }
    deactivate();
    return true;
}

bool Bullet::cullOutside(const sf::FloatRect& worldBounds,
                         float margin) noexcept {
    if (!active) return false;
    const float safeMargin = std::max(0.f, margin);
    const sf::FloatRect paddedWorld(
        {worldBounds.position.x - safeMargin,
         worldBounds.position.y - safeMargin},
        {worldBounds.size.x + 2.f * safeMargin,
         worldBounds.size.y + 2.f * safeMargin});
    if (paddedWorld.findIntersection(hitbox.getGlobalBounds())) return false;
    deactivate();
    return true;
}

