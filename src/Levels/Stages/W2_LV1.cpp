#include "Core/stdafx.hpp"

W2_LV1::W2_LV1(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(2, 1, players) {
    if (!isLoaded()) return;

    addStageEnemy("Koopa", {6138.f, 768.f});
    addStageEnemy("Koopa", {6238.f, 768.f});
    addStageEnemy("Goomba", {6738.f, 768.f});
    addStageEnemy("Goomba", {6838.f, 768.f});
    addStageEnemy("Koopa", {580.f, 448.f}, 572.f, 1088.f);
    addStageEnemy("Goomba", {590.f, 448.f}, 572.f, 1088.f);
    addStageEnemy("Koopa", {580.f, 192.f}, 572.f, 1088.f);
    addStageEnemy("Goomba", {590.f, 192.f}, 572.f, 1088.f);
    addStageEnemy("Koopa", {1800.f, 768.f}, 1664.f, 2156.f);
    addStageEnemy("Koopa", {6100.f, 320.f}, 6088.f, 6929.f);
    addStageEnemy("Goomba", {6300.f, 320.f}, 6088.f, 6929.f);
    addStageEnemy("Goomba", {9000.f, 768.f}, 8588.f, 10368.f);
    addStageEnemy("Goomba", {9100.f, 768.f}, 8588.f, 10368.f);
    addStageEnemy("Koopa", {9300.f, 768.f}, 8588.f, 10368.f);
    addStageEnemy("Koopa", {9400.f, 768.f}, 8588.f, 10368.f);
    addStageEnemy("Koopa", {9800.f, 320.f}, 9600.f, 10432.f);
    addStageEnemy("Goomba", {10000.f, 320.f}, 9600.f, 10432.f);
    addStageEnemy("Koopa", {10100.f, 320.f}, 9600.f, 10432.f);
    addStageEnemy("Koopa", {11300.f, 768.f}, 11206.f, 11648.f);

    addStageMovingBlock({3974.f, 200.f}, "snow_ground_mid");
}

void W2_LV1::update(float dt) {
    const float currentDt = firstUpdate ? 0.f : dt;
    firstUpdate = false;

    std::vector<bool> previousStates;
    previousStates.reserve(getPlayers().size());
    for (PlayerManager* player : getPlayers()) {
        previousStates.push_back(player->areHorizontalControlsInverted());
        player->setHorizontalControlsInverted(true);
    }

    ConfiguredLevel::update(currentDt);

    for (std::size_t i = 0; i < getPlayers().size(); ++i) {
        getPlayers()[i]->setHorizontalControlsInverted(previousStates[i]);
    }
}
