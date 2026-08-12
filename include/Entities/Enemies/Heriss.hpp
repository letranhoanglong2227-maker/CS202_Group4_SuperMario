#pragma once

#include "Entities/Base/Enemy.hpp"

class Heriss : public Enemy {
public:
    Heriss(const sf::Vector2f& pos = { 0.f, 0.f });
    ~Heriss() override = default;

    void onStomped() override; // Spiked enemy cannot be stomped safely
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
