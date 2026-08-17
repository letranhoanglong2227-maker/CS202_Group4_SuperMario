#include "Objects/Environment/Rocket.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <cmath>

Rocket::Rocket(sf::Vector2f pos, const GameObject* followTarget, float moveSpeed)
    : target(followTarget), speed(std::max(0.f, moveSpeed)) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE * 0.5f});
    shape.setPosition(pos);
    shape.setSize(getSize());
    shape.setFillColor(sf::Color(235, 235, 235));
}

void Rocket::setTarget(const GameObject* newTarget) noexcept { target = newTarget; }

void Rocket::update(float dt) {
    if (dt <= 0.f) return;
    if (target) {
        sf::Vector2f offset = target->getCenter() - getCenter();
        const float length = std::sqrt(offset.x * offset.x + offset.y * offset.y);
        if (length > 0.001f) velocity = offset / length * speed;
    }
    setPosition(getPosition() + velocity * dt);
    shape.setPosition(getPosition());
}

void Rocket::render(sf::RenderTarget* renderTarget) {
    if (renderTarget) renderTarget->draw(shape);
}

