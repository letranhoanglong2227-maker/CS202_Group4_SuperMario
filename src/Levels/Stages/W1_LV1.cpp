#include "Core/stdafx.hpp"

W1_LV1::W1_LV1(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(1, 1, players) {
    if (!isLoaded()) return;

    addStageEnemy("Koopa", {700.f, 768.f});
    addStageEnemy("Koopa", {2037.f, 768.f});
    addStageEnemy("Koopa", {3200.f, 768.f});
    addStageEnemy("Goomba", {4000.f, 768.f}, 3776.f, 4335.f);
    addStageEnemy("Goomba", {4600.f, 768.f}, 4552.f, 5421.f);
    addStageEnemy("Koopa", {4700.f, 768.f}, 4552.f, 5421.f);
    addStageEnemy("Goomba", {5200.f, 256.f}, 5142.f, 5571.f);
    addStageEnemy("Goomba", {6200.f, 512.f}, 6032.f, 6451.f);
    addStageEnemy("Goomba", {5800.f, 768.f}, 5706.f, 8512.f);
    addStageEnemy("Goomba", {5900.f, 768.f}, 5706.f, 8512.f);
    addStageEnemy("Koopa", {7100.f, 768.f}, 5706.f, 8512.f);
    addStageEnemy("Koopa", {7200.f, 768.f}, 5706.f, 8512.f);
    addStageEnemy("Koopa", {10691.f, 768.f});
    addStageEnemy("Koopa", {10800.f, 512.f}, 10764.f, 10940.f);
    addStageEnemy("Goomba", {9241.f, 768.f});
    addStageEnemy("Koopa", {8200.f, 256.f}, 8184.f, 8374.f);
    addStageEnemy("Koopa", {6800.f, 512.f}, 6787.f, 7157.f);
    addStageEnemy("Goomba", {7700.f, 512.f}, 7566.f, 7865.f);
    addStageEnemy("Goomba", {7600.f, 256.f}, 7555.f, 7878.f);
    addStageEnemy("Koopa", {1300.f, 256.f}, 1294.f, 1538.f);
}
