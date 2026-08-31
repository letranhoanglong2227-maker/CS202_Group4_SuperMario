#include "Objects/Environment/WinFlag.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Objects/Blocks/Block.hpp"
#include <algorithm>
#include <utility>

namespace {
constexpr float FLAG_SLIDE_DURATION = 1.f;
constexpr float FLAG_TRAVEL = 470.f;
}

WinFlag::WinFlag(sf::Vector2f pos, CompletionCallback completion)
    : callback(std::move(completion)) {
    setSize({MapFormat::TILE_SIZE, MapFormat::TILE_SIZE});
    sf::Texture& tileset = TextureBlockManager::getBlocksTexture();
    entitySprite.setTexture(tileset);
    entitySprite.setTextureRect({{1, 341}, {14, 16}});
    entitySprite.setScale({MapFormat::TILE_SIZE / 14.f, 4.f});
    poleSprite.emplace(tileset);
    poleSprite->setTextureRect({{42, 138}, {2, 16}});
    poleSprite->setScale({8.f, 34.f});
    sphereSprite.emplace(tileset);
    sphereSprite->setTextureRect({{5, 129}, {8, 8}});
    sphereSprite->setScale({4.f, 4.f});
    setPosition(pos);
}

void WinFlag::update(float dt) {
    if (!activated || completed || dt <= 0.f) return;

    animationTime = std::min(animationTime + dt, FLAG_SLIDE_DURATION);
    const int nextFrame = static_cast<int>(animationTime / 0.1f) % 2;
    if (nextFrame != currentFrame) {
        currentFrame = nextFrame;
        entitySprite.setTextureRect(
            currentFrame == 0 ? sf::IntRect{{1, 341}, {14, 16}}
                              : sf::IntRect{{18, 341}, {15, 16}});
    }
    syncGeometry();
    if (animationTime < FLAG_SLIDE_DURATION) return;

    completed = true;
    if (callback) callback();
}

void WinFlag::render(sf::RenderTarget* target) {
    if (!target) return;
    target->draw(entitySprite);
    if (poleSprite) target->draw(*poleSprite);
    if (sphereSprite) target->draw(*sphereSprite);
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

sf::Vector2f WinFlag::getFlagVisualPosition() const noexcept {
    return entitySprite.getPosition();
}

void WinFlag::syncGeometry() {
    const sf::Vector2f baseAnchor = getPosition();
    hitbox.setPosition(baseAnchor);
    if (poleSprite)
        poleSprite->setPosition({baseAnchor.x + 22.f,
                                 baseAnchor.y - FLAG_TRAVEL});
    if (sphereSprite)
        sphereSprite->setPosition({baseAnchor.x + 14.f,
                                   baseAnchor.y - 490.f});

    const float progress = std::clamp(
        animationTime / FLAG_SLIDE_DURATION, 0.f, 1.f);
    entitySprite.setPosition(
        {baseAnchor.x + 25.f,
         baseAnchor.y - FLAG_TRAVEL * progress});
}

