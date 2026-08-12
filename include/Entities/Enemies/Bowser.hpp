#pragma once

#include "Entities/Base/Enemy.hpp"

class Bowser : public Enemy {
private:
    float attackTimer{ 0.f };
    float fireTimer{ 0.f };
    bool breathingFire{ false };
    bool jumpingSlam{ false };

public:
    Bowser(const sf::Vector2f& pos = { 0.f, 0.f });
    ~Bowser() override = default;

    bool isBreathingFire() const;
    void breatheFire();
    void jumpSlam();

    void takeDamage(int amount) override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
