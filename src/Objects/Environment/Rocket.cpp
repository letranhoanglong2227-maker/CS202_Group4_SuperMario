#include "Objects/Environment/Rocket.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Entities/Base/Enemy.hpp"
#include "Physics/PhysicsEngine.hpp"
#include <algorithm>
#include <cmath>
#include <utility>

Rocket::Rocket(sf::Vector2f pos, TargetResolver resolver, float moveSpeed,
               float activeLifetime)
    : targetResolver(std::move(resolver)), speed(std::max(0.f, moveSpeed)),
      lifetime(std::max(0.f, activeLifetime)) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE - 2.f, MapFormat::TILE_SIZE - 2.f});
    previousBounds = hitbox.getGlobalBounds();
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(235, 235, 235));
    entitySprite.setTexture(TextureEnemyManager::getEnemyTexture(), true);
    entitySprite.setTextureRect({{87, 63}, {16, 16}});
    entitySprite.setScale({getSize().x / 16.f, getSize().y / 16.f});
    entitySprite.setPosition(pos);
    active = activeLifetime > 0.f;
}

Rocket::Rocket(sf::Vector2f pos, sf::Vector2f initialVelocity,
               float activeLifetime)
    : Rocket(pos, TargetResolver{}, 0.f, activeLifetime) {
    velocity = initialVelocity;
}

void Rocket::setTargetResolver(TargetResolver resolver) {
    targetResolver = std::move(resolver);
}

void Rocket::update(float dt) {
    if (!active || dt <= 0.f) return;
    previousBounds = hitbox.getGlobalBounds();
    lifetime -= dt;
    if (lifetime <= 0.f) {
        deactivate();
        return;
    }

    if (targetResolver) {
        const std::optional<sf::Vector2f> targetPosition = targetResolver();
        if (!targetPosition) {
            deactivate();
            return;
        }
        sf::Vector2f offset = *targetPosition - getCenter();
        const float length = std::sqrt(offset.x * offset.x + offset.y * offset.y);
        velocity = length > 0.001f ? offset / length * speed : sf::Vector2f{};
    }
    setPosition(getPosition() + velocity * dt);
    shape.setPosition(getPosition());
    entitySprite.setTextureRect(
        velocity.x >= 0.f ? sf::IntRect({35, 63}, {16, 16})
                          : sf::IntRect({87, 63}, {16, 16}));
    entitySprite.setPosition(getPosition());
}

void Rocket::render(sf::RenderTarget* renderTarget) {
    if (renderTarget && active) renderTarget->draw(entitySprite);
}

bool Rocket::isActive() const noexcept { return active; }

void Rocket::deactivate() noexcept { active = false; }

bool Rocket::deactivateOnWorldCollision(
    const sf::FloatRect& obstacle) noexcept {
    if (!active || !PhysicsEngine::sweptAabbIntersects(
                       previousBounds, hitbox.getGlobalBounds(), obstacle)) {
        return false;
    }
    deactivate();
    return true;
}

sf::FloatRect Rocket::getPreviousBounds() const noexcept {
    return previousBounds;
}

bool Rocket::cullOutside(const sf::FloatRect& worldBounds,
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

