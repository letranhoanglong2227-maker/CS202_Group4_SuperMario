#include "Objects/Environment/Rocket.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <cmath>
#include <utility>

Rocket::Rocket(sf::Vector2f pos, TargetResolver resolver, float moveSpeed,
               float activeLifetime)
    : targetResolver(std::move(resolver)), speed(std::max(0.f, moveSpeed)),
      lifetime(std::max(0.f, activeLifetime)) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE * 0.5f});
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(235, 235, 235));
}

void Rocket::setTargetResolver(TargetResolver resolver) {
    targetResolver = std::move(resolver);
}

void Rocket::update(float dt) {
    if (!active || dt <= 0.f) return;
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
        if (length > 0.001f) velocity = offset / length * speed;
    }
    setPosition(getPosition() + velocity * dt);
    shape.setPosition(getPosition());
}

void Rocket::render(sf::RenderTarget* renderTarget) {
    if (renderTarget && active) renderTarget->draw(shape);
}

bool Rocket::isActive() const noexcept { return active; }

void Rocket::deactivate() noexcept { active = false; }

