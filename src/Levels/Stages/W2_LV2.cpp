#include "Core/stdafx.hpp"

W2_LV2::W2_LV2(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(2, 2, players) {
    if (!isLoaded()) return;

    addStageEnemy("Koopa", {10847.f, 768.f});
    addStageEnemy("Koopa", {10847.f, 768.f});
    addStageEnemy("Goomba", {10447.f, 768.f});
    addStageEnemy("Goomba", {10347.f, 768.f});
    addStageEnemy("Koopa", {1700.f, 768.f}, 1600.f, 2174.f);
    addStageEnemy("Goomba", {3400.f, 512.f}, 3390.f, 4087.f);
    addStageEnemy("Koopa", {3500.f, 512.f}, 3390.f, 4087.f);
    addStageEnemy("Koopa", {4600.f, 576.f}, 4552.f, 4922.f);
    addStageEnemy("Goomba", {5800.f, 768.f}, 5700.f, 7482.f);
    addStageEnemy("Goomba", {5900.f, 768.f}, 5700.f, 7482.f);
    addStageEnemy("Koopa", {6200.f, 768.f}, 5700.f, 7482.f);
    addStageEnemy("Koopa", {7000.f, 768.f}, 5700.f, 7482.f);
    addStageEnemy("Koopa", {6200.f, 320.f}, 6074.f, 6919.f);
    addStageEnemy("Goomba", {6500.f, 320.f}, 6074.f, 6919.f);

    addStageMovingBlock({2200.f, 300.f}, "snow_ground_mid");
    addStageMovingBlock({2800.f, 300.f}, "snow_ground_mid");
}
