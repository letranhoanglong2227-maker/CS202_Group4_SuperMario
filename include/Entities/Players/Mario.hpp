#pragma once

#include "Entities/Players/PlayerManager.hpp"

class Mario : public PlayerManager {
public:
    Mario();
    ~Mario() override = default;

    void updateAnimation(float dt) override;
};
