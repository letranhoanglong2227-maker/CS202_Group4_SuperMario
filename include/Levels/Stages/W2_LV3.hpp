#pragma once

#include "Levels/Managers/LevelManager.hpp"
class W2_LV3 : public ConfiguredLevel {
public: explicit W2_LV3(const std::vector<PlayerManager*>& p = {}) : ConfiguredLevel(2, 3, p) {}
};

