#include "Objects/Environment/Bullet.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Objects/Items/PowerUpObject.hpp"
#include "Physics/PhysicsEngine.hpp"
#include <algorithm>
#include <array>

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f initialVelocity,
               float activeLifetime, int impactDamage)
    : velocity(initialVelocity), shape(MapFormat::TILE_SIZE * 0.25f),
      lifetime(std::max(0.f, activeLifetime)),
      damage(std::max(0, impactDamage)), active(activeLifetime > 0.f) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE});
    previousBounds = hitbox.getGlobalBounds();
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(40, 40, 40));
    entitySprite.setTexture(TextureItemManager::getItemTexture(), true);
    entitySprite.setTextureRect({{2, 70}, {8, 8}});
    entitySprite.setScale({getSize().x / 8.f, getSize().y / 8.f});
    entitySprite.setPosition(pos);
}

void Bullet::update(float dt) {
    if (!active || dt <= 0.f) return;
    previousBounds = hitbox.getGlobalBounds();
    setPosition(getPosition() + velocity * dt);
    shape.setPosition(getPosition());
    animationTime += dt;
    constexpr std::array frames{
        sf::Vector2i{2, 70}, sf::Vector2i{11, 70},
        sf::Vector2i{20, 70}, sf::Vector2i{29, 70}};
    const auto frame = static_cast<std::size_t>(animationTime / 0.1f) %
                       frames.size();
    entitySprite.setTextureRect({frames[frame], {8, 8}});
    entitySprite.setPosition(getPosition());
    lifetime -= dt;
    if (lifetime <= 0.f) active = false;
}

void Bullet::render(sf::RenderTarget* target) {
    if (target && active) target->draw(entitySprite);
}

bool Bullet::isActive() const noexcept { return active; }
void Bullet::deactivate() noexcept { active = false; }

bool Bullet::deactivateOnWorldCollision(
    const sf::FloatRect& obstacle) noexcept {
    if (!active || !PhysicsEngine::sweptAabbIntersects(
                       previousBounds, hitbox.getGlobalBounds(), obstacle)) {
        return false;
    }
    deactivate();
    return true;
}

sf::FloatRect Bullet::getPreviousBounds() const noexcept {
    return previousBounds;
}

int Bullet::getDamage() const noexcept { return damage; }

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

