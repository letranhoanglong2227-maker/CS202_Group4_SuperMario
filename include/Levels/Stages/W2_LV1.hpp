#pragma once

#include "Levels/Managers/LevelManager.hpp"

class W2_LV1 : public ConfiguredLevel {
public:
    explicit W2_LV1(const std::vector<PlayerManager*>& players = {});
    void update(float dt) override;

private:
    bool firstUpdate{true};
};
