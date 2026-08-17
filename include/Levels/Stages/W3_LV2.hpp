#pragma once

#include "Levels/Managers/LevelManager.hpp"
class W3_LV2 : public ConfiguredLevel {
public: explicit W3_LV2(const std::vector<PlayerManager*>& p = {}) : ConfiguredLevel(3, 2, p) {}
};

