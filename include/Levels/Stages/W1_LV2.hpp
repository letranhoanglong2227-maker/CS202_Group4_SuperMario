#pragma once

#include "Levels/Managers/LevelManager.hpp"

class W1_LV2 : public ConfiguredLevel {
public:
    explicit W1_LV2(const std::vector<PlayerManager*>& players = {});
};
