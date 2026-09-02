#pragma once

#include "Levels/Stages/W1_LV1.hpp"
#include "Levels/Stages/W1_LV2.hpp"
#include "Levels/Stages/W1_LV3.hpp"
#include "Levels/Stages/W2_LV1.hpp"
#include "Levels/Stages/W2_LV2.hpp"
#include "Levels/Stages/W2_LV3.hpp"
#include "Levels/Stages/W3_LV1.hpp"
#include "Levels/Stages/W3_LV2.hpp"
#include "Levels/Stages/W3_LV3.hpp"

#include <memory>
#include <vector>

inline std::unique_ptr<LevelManager> createConfiguredLevel(
    int world, int level, const std::vector<PlayerManager*>& players) {
    if (world == 1 && level == 1) return std::make_unique<W1_LV1>(players);
    if (world == 1 && level == 2) return std::make_unique<W1_LV2>(players);
    if (world == 1 && level == 3) return std::make_unique<W1_LV3>(players);
    if (world == 2 && level == 1) return std::make_unique<W2_LV1>(players);
    if (world == 2 && level == 2) return std::make_unique<W2_LV2>(players);
    if (world == 2 && level == 3) return std::make_unique<W2_LV3>(players);
    if (world == 3 && level == 1) return std::make_unique<W3_LV1>(players);
    if (world == 3 && level == 2) return std::make_unique<W3_LV2>(players);
    if (world == 3 && level == 3) return std::make_unique<W3_LV3>(players);
    return nullptr;
}
