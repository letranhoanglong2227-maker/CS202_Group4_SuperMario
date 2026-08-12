#pragma once

#include "Entities/Base/Enemy.hpp"

class PeteyPiranha : public Enemy {
private:
    float emergeTimer{ 0.f };
    bool emerged{ false };
    float baseY{ 0.f };

public:
    PeteyPiranha(const sf::Vector2f& pos = { 0.f, 0.f });
    ~PeteyPiranha() override = default;

    bool isEmerged() const;
    void emerge();
    void retract();

    void takeDamage(int amount) override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
