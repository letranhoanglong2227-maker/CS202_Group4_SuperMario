#pragma once

#include "Entities/Players/PlayerManager.hpp"

class Luigi : public PlayerManager {
public:
    Luigi();
    ~Luigi() override = default;

    void updateAnimation(float dt) override;
};
