#include "Core/stdafx.hpp"

namespace {
constexpr float LEGACY_SCREEN_HEIGHT = 960.f;

class BossMushroom final : public Mushroom {
public:
    using Mushroom::Mushroom;
};

std::string blindFoldPath() {
    const std::string sourcePath = "assets/textures/BlindFold.png";
    return std::filesystem::exists(sourcePath) ? sourcePath
                                                : "../" + sourcePath;
}
}

W3_LV3::W3_LV3(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(3, 3, players), blindFold(blindFoldTexture) {
    if (!isLoaded()) return;

    addStageEnemy("Bowser", {500.f, 250.f}, 374.f, 1120.f);

    blindFoldLoaded = blindFoldTexture.loadFromFile(blindFoldPath());
    if (blindFoldLoaded) {
        blindFold.setTexture(blindFoldTexture, true);
        const float height = blindFold.getGlobalBounds().size.y;
        if (height > 0.f) {
            const float scale = 4.f * LEGACY_SCREEN_HEIGHT / height;
            blindFold.setScale({scale, scale});
        }
    }
}

void W3_LV3::updateMushroom(float dt) {
    if (dt <= 0.f) return;
    reloadMushroomTimer += dt;
    if (reloadMushroomTimer < reloadMushroom) return;
    reloadMushroomTimer = 0.f;

    for (const auto& entity : getEntities()) {
        if (auto* mushroom = dynamic_cast<BossMushroom*>(entity.get())) {
            mushroom->setExist(false);
        }
    }

    static std::mt19937 randomEngine{std::random_device{}()};
    std::uniform_int_distribution<int> spawnX(374, 1120);
    addEntity(std::make_unique<BossMushroom>(
        sf::Vector2f{static_cast<float>(spawnX(randomEngine)), 768.f}));
}

void W3_LV3::update(float dt) {
    updateMushroom(dt);
    ConfiguredLevel::update(dt);
    positionBlindFold();
}

void W3_LV3::render(sf::RenderTarget* target) {
    ConfiguredLevel::render(target);
}

void W3_LV3::positionBlindFold() {
    if (!blindFoldLoaded || getPlayers().empty()) return;
    const sf::FloatRect bounds = blindFold.getGlobalBounds();
    const sf::Vector2f playerPosition = getPlayers().front()->getPosition();
    blindFold.setPosition(
        {playerPosition.x - bounds.size.x / 2.f - CELL_SIZE,
         playerPosition.y - bounds.size.y / 2.f + 2.f * CELL_SIZE});
}
