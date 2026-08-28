#include "Objects/Environment/WinFlag.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <utility>

namespace {
constexpr float FLAG_SLIDE_DURATION = 1.f;
}

WinFlag::WinFlag(sf::Vector2f pos, CompletionCallback completion)
    : callback(std::move(completion)) {
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE * 4.f});
    pole.setSize({4.f, getSize().y});
    pole.setFillColor(sf::Color(230, 230, 230));
    flag.setPointCount(3);
    flag.setPoint(0, {4.f, 0.f});
    flag.setPoint(1, {MapFormat::TILE_SIZE, MapFormat::TILE_SIZE * 0.5f});
    flag.setPoint(2, {4.f, MapFormat::TILE_SIZE});
    flag.setFillColor(sf::Color(45, 190, 70));
    setPosition(pos);
}

void WinFlag::update(float dt) {
    if (!activated || completed || dt <= 0.f) return;

    animationTime = std::min(animationTime + dt, FLAG_SLIDE_DURATION);
    syncGeometry();
    if (animationTime < FLAG_SLIDE_DURATION) return;

    completed = true;
    if (callback) callback();
}

void WinFlag::render(sf::RenderTarget* target) {
    if (!target) return;
    target->draw(pole);
    target->draw(flag);
}

void WinFlag::setPosition(const sf::Vector2f& baseAnchor) {
    GameObject::setPosition(baseAnchor);
    syncGeometry();
}

void WinFlag::activate() {
    if (activated) return;
    activated = true;
}

bool WinFlag::isActivated() const noexcept { return activated; }

void WinFlag::syncGeometry() {
    const sf::Vector2f baseAnchor = getPosition();
    const float top = baseAnchor.y - getSize().y;
    hitbox.setPosition({baseAnchor.x, top});
    pole.setPosition({baseAnchor.x, top});

    const float progress = std::clamp(
        animationTime / FLAG_SLIDE_DURATION, 0.f, 1.f);
    flag.setPosition({baseAnchor.x,
                      top + (getSize().y - MapFormat::TILE_SIZE) * progress});
}

