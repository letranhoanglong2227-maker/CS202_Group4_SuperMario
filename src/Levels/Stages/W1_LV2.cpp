#include "Core/stdafx.hpp"

W1_LV2::W1_LV2(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(1, 2, players) {
    if (!isLoaded()) return;

    addStageEnemy("Koopa", {769.f, 768.f});
    addStageEnemy("Goomba", {12015.f, 768.f});
    addStageEnemy("PeteyPiranha", {4670.f, 768.f});
    addStageEnemy("PeteyPiranha", {2000.f, 768.f});

    addStageEnemy("Koopa", {1100.f, 320.f}, 1053.f, 1487.f);
    addStageEnemy("Goomba", {5500.f, 576.f}, 5459.f, 5888.f);
    addStageEnemy("Goomba", {6200.f, 384.f}, 5914.f, 6581.f);
    addStageEnemy("FlyingKoopa", {5200.f, 400.f}, 5150.f, 5500.f);
    addStageEnemy("Koopa", {6400.f, 384.f}, 5914.f, 6581.f);
    addStageEnemy("Koopa", {6100.f, 128.f}, 6016.f, 6468.f);
    addStageEnemy("Goomba", {6700.f, 512.f}, 6592.f, 7085.f);
    addStageEnemy("Koopa", {7800.f, 768.f}, 7636.f, 8064.f);
    addStageEnemy("Goomba", {8500.f, 512.f}, 8441.f, 9040.f);
    addStageEnemy("Koopa", {8600.f, 512.f}, 8441.f, 9040.f);
    addStageEnemy("FlyingKoopa", {8500.f, 200.f}, 8441.f, 9040.f);
    addStageEnemy("FlyingKoopa", {10700.f, 300.f}, 10400.f, 11000.f);

    addStageMovingBlock({4500.f, 200.f}, "basic_ground_mid");
}
