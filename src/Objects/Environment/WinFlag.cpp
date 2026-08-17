#include "Objects/Environment/WinFlag.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <utility>

WinFlag::WinFlag(sf::Vector2f pos, CompletionCallback completion)
    : callback(std::move(completion)) {
    setPosition(pos);
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE * 4.f});
    pole.setPosition(pos);
    pole.setSize({4.f, getSize().y});
    pole.setFillColor(sf::Color(230, 230, 230));
    flag.setPointCount(3);
    flag.setPoint(0, {4.f, 0.f});
    flag.setPoint(1, {MapFormat::TILE_SIZE, MapFormat::TILE_SIZE * 0.5f});
    flag.setPoint(2, {4.f, MapFormat::TILE_SIZE});
    flag.setPosition(pos);
    flag.setFillColor(sf::Color(45, 190, 70));
}

void WinFlag::update(float dt) { (void)dt; }

void WinFlag::render(sf::RenderTarget* target) {
    if (!target) return;
    target->draw(pole);
    target->draw(flag);
}

void WinFlag::activate() {
    if (activated) return;
    activated = true;
    if (callback) callback();
}

bool WinFlag::isActivated() const noexcept { return activated; }

