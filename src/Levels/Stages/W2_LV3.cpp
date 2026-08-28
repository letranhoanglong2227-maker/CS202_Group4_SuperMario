#include "Core/stdafx.hpp"

W2_LV3::W2_LV3(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(2, 3, players) {
    if (!isLoaded()) return;

    addStageEnemy("Goomba", {6231.f, 448.f});
    addStageEnemy("Koopa", {6231.f, 448.f});
    addStageEnemy("Goomba", {1100.f, 384.f}, 923.f, 1466.f);
    addStageEnemy("Koopa", {1200.f, 384.f}, 923.f, 1466.f);
    addStageEnemy("Goomba", {1800.f, 256.f}, 1736.f, 2941.f);
    addStageEnemy("Koopa", {1900.f, 256.f}, 1736.f, 2941.f);
    addStageEnemy("Goomba", {3300.f, 192.f}, 3208.f, 4224.f);
    addStageEnemy("Koopa", {3400.f, 192.f}, 3208.f, 4224.f);
    addStageEnemy("Goomba", {3500.f, 448.f}, 3008.f, 4662.f);
    addStageEnemy("Koopa", {3600.f, 448.f}, 3008.f, 4662.f);
    addStageEnemy("Goomba", {3800.f, 448.f}, 3008.f, 4662.f);
    addStageEnemy("Koopa", {3900.f, 448.f}, 3008.f, 4662.f);
    addStageEnemy("Goomba", {7900.f, 192.f}, 7873.f, 10107.f);
    addStageEnemy("Koopa", {8000.f, 192.f}, 7873.f, 10107.f);
    addStageEnemy("Goomba", {8100.f, 192.f}, 7873.f, 10107.f);
    addStageEnemy("Koopa", {8200.f, 192.f}, 7873.f, 10107.f);
    addStageEnemy("Goomba", {7500.f, 768.f}, 6971.f, 10113.f);
    addStageEnemy("Koopa", {8100.f, 768.f}, 6971.f, 10113.f);
    addStageEnemy("Goomba", {8400.f, 768.f}, 6971.f, 10113.f);
    addStageEnemy("Koopa", {8800.f, 768.f}, 6971.f, 10113.f);
    addStageEnemy("Goomba", {9200.f, 768.f}, 6971.f, 10113.f);
    addStageEnemy("Koopa", {9500.f, 768.f}, 6971.f, 10113.f);
    addStageEnemy("Koopa", {11000.f, 768.f}, 10826.f, 11648.f);
    addStageEnemy("Koopa", {5200.f, 448.f}, 5114.f, 5568.f);
    addStageEnemy("Goomba", {5200.f, 768.f}, 5139.f, 5754.f);
    addStageEnemy("Koopa", {5400.f, 768.f}, 5139.f, 5754.f);

    addStageMovingBlock({10400.f, 300.f}, "snow_ground_mid");
}
