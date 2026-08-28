#include "Core/stdafx.hpp"

W3_LV1::W3_LV1(const std::vector<PlayerManager*>& players)
    : ConfiguredLevel(3, 1, players) {
    if (!isLoaded()) return;

    addStageEnemy("Goomba", {2100.f, 576.f}, 2013.f, 2545.f);
    addStageEnemy("Heriss", {300.f, 500.f}, 100.f, 550.f);
    addStageEnemy("Goomba", {9200.f, 134.f}, 9197.f, 9855.f);
    addStageEnemy("Koopa", {9400.f, 134.f}, 9197.f, 9855.f);
    addStageEnemy("Heriss", {9600.f, 134.f}, 9197.f, 9855.f);
    addStageEnemy("Koopa", {11200.f, 582.f}, 11095.f, 11392.f);
    addStageEnemy("Heriss", {11250.f, 582.f}, 11095.f, 11392.f);
    addStageEnemy("Koopa", {3300.f, 576.f}, 3218.f, 3400.f);
    addStageEnemy("Heriss", {5700.f, 640.f}, 5500.f, 5900.f);
}
