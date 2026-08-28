#include "Core/stdafx.hpp"

W1_LV3::W1_LV3(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(1, 3, players) {
    if (!isLoaded()) return;

    addStageMovingBlock({4200.f, 200.f}, "basic_leafground_mid");
    addStageMovingBlock({4800.f, 200.f}, "basic_leafground_mid");
    addStageMovingBlock({7400.f, 400.f}, "basic_leafground_mid");
    addStageMovingBlock({8000.f, 400.f}, "basic_leafground_mid");
    addStageMovingBlock({8600.f, 400.f}, "basic_leafground_mid");
    addStageMovingBlock({9200.f, 400.f}, "basic_leafground_mid");
    addStageMovingBlock({9800.f, 400.f}, "basic_leafground_mid");

    addStageEnemy("Goomba", {400.f, 704.f}, 32.f, 576.f);
    addStageEnemy("Goomba", {600.f, 576.f}, 575.f, 1344.f);
    addStageEnemy("Koopa", {700.f, 576.f}, 575.f, 1344.f);
    addStageEnemy("Goomba", {1400.f, 384.f}, 1360.f, 2032.f);
    addStageEnemy("Koopa", {1400.f, 384.f}, 1360.f, 2032.f);
    addStageEnemy("Goomba", {2800.f, 320.f}, 2565.f, 3249.f);
    addStageEnemy("Koopa", {2800.f, 320.f}, 2565.f, 3249.f);
    addStageEnemy("Koopa", {3800.f, 704.f}, 3752.f, 4144.f);
    addStageEnemy("FlyingKoopa", {7700.f, 300.f}, 7450.f, 7950.f);
    addStageEnemy("FlyingKoopa", {8400.f, 300.f}, 8050.f, 8550.f);
    addStageEnemy("FlyingKoopa", {8900.f, 300.f}, 8650.f, 9150.f);
    addStageEnemy("FlyingKoopa", {9500.f, 300.f}, 9250.f, 9750.f);
    addStageEnemy("Goomba", {6800.f, 768.f}, 6784.f, 7281.f);
    addStageEnemy("Goomba", {10500.f, 768.f}, 10442.f, 11328.f);
    addStageEnemy("Koopa", {10700.f, 768.f}, 10442.f, 11328.f);
    addStageEnemy("Koopa", {10700.f, 768.f}, 10442.f, 11328.f);
    addStageEnemy("Koopa", {10900.f, 768.f}, 10442.f, 11328.f);
    addStageEnemy("Koopa", {12075.f, 768.f});
}
