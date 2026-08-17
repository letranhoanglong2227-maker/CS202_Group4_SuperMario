#pragma once

#include "Levels/Managers/LevelManager.hpp"
class W3_LV1 : public ConfiguredLevel {
public: explicit W3_LV1(const std::vector<PlayerManager*>& p = {}) : ConfiguredLevel(3, 1, p) {}
};

