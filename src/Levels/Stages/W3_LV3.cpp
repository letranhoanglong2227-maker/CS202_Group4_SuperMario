#include "Core/stdafx.hpp"
#include "Core/AssetLocator.hpp"

namespace {
constexpr float LEGACY_SCREEN_HEIGHT = 960.f;

class BossMushroom final : public Mushroom {
public:
    using Mushroom::Mushroom;
};

}

W3_LV3::W3_LV3(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(3, 3, players), blindFold(blindFoldTexture) {
    if (!isLoaded()) return;

    addStageEnemy("Bowser", {500.f, 250.f}, 374.f, 1120.f);

    const auto blindFoldPath = AssetLocator::find(
        "assets/textures/BlindFold.png");
    blindFoldLoaded = blindFoldPath &&
                      blindFoldTexture.loadFromFile(*blindFoldPath);
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
    const sf::Vector2f playerCenter = getPlayers().front()->getCenter();
    blindFold.setPosition(
        {playerCenter.x - bounds.size.x / 2.f,
         playerCenter.y - bounds.size.y / 2.f + 2.f * CELL_SIZE});
}
