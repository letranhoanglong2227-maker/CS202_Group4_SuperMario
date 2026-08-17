#pragma once

#include "Levels/Managers/LevelManager.hpp"
class W2_LV2 : public ConfiguredLevel {
public: explicit W2_LV2(const std::vector<PlayerManager*>& p = {}) : ConfiguredLevel(2, 2, p) {}
};

