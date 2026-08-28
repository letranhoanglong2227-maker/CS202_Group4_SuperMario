#pragma once

#include "Levels/Managers/LevelManager.hpp"

class W1_LV1 : public ConfiguredLevel {
public:
    explicit W1_LV1(const std::vector<PlayerManager*>& players = {});
};
