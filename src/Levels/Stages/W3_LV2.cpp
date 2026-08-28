#include "Core/stdafx.hpp"

namespace {
constexpr float LEGACY_SCREEN_HEIGHT = 960.f;

std::string blindFoldPath() {
    const std::string sourcePath = "assets/textures/BlindFold.png";
    return std::filesystem::exists(sourcePath) ? sourcePath
                                                : "../" + sourcePath;
}
}

W3_LV2::W3_LV2(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(3, 2, players), blindFold(blindFoldTexture) {
    if (!isLoaded()) return;

    blindFoldLoaded = blindFoldTexture.loadFromFile(blindFoldPath());
    if (blindFoldLoaded) {
        blindFold.setTexture(blindFoldTexture, true);
        const float height = blindFold.getGlobalBounds().size.y;
        if (height > 0.f) {
            const float scale = 4.f * LEGACY_SCREEN_HEIGHT / height;
            blindFold.setScale({scale, scale});
        }
    }

    addStageEnemy("Koopa", {2200.f, 576.f}, 2000.f, 2550.f);
    addStageEnemy("Goomba", {3300.f, 134.f}, 3200.f, 5883.f);
    addStageEnemy("Koopa", {3400.f, 134.f}, 3200.f, 5883.f);
    addStageEnemy("Goomba", {3600.f, 134.f}, 3200.f, 5883.f);
    addStageEnemy("Koopa", {3800.f, 134.f}, 3200.f, 5883.f);
    addStageEnemy("Goomba", {6300.f, 640.f}, 6272.f, 7021.f);
    addStageEnemy("Koopa", {6600.f, 640.f}, 6272.f, 7021.f);
    addStageEnemy("Goomba", {8500.f, 448.f}, 8450.f, 9149.f);
    addStageEnemy("Koopa", {9000.f, 448.f}, 8450.f, 9149.f);
    addStageEnemy("Goomba", {8300.f, 640.f}, 8287.f, 9536.f);
    addStageEnemy("Koopa", {8500.f, 640.f}, 8287.f, 9536.f);
    addStageEnemy("Goomba", {8700.f, 640.f}, 8287.f, 9536.f);
    addStageEnemy("Koopa", {9100.f, 640.f}, 8287.f, 9536.f);
    addStageEnemy("Koopa", {11200.f, 640.f}, 11083.f, 11392.f);
}

void W3_LV2::update(float dt) {
    ConfiguredLevel::update(dt);
    positionBlindFold();
}

void W3_LV2::render(sf::RenderTarget* target) {
    ConfiguredLevel::render(target);
    if (target && blindFoldLoaded) target->draw(blindFold);
}

void W3_LV2::positionBlindFold() {
    if (!blindFoldLoaded || getPlayers().empty()) return;
    const sf::FloatRect bounds = blindFold.getGlobalBounds();
    const sf::Vector2f playerPosition = getPlayers().front()->getPosition();
    blindFold.setPosition(
        {playerPosition.x - bounds.size.x / 2.f - CELL_SIZE,
         playerPosition.y - bounds.size.y / 2.f + 2.f * CELL_SIZE});
}
