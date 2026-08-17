#pragma once

#include "Levels/Managers/LevelManager.hpp"
class W3_LV3 : public ConfiguredLevel {
public: explicit W3_LV3(const std::vector<PlayerManager*>& p = {}) : ConfiguredLevel(3, 3, p) {}
};

