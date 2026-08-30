#pragma once

#include "Entities/Base/Enemy.hpp"

class Goomba : public Enemy {
private:
    bool isSquished{ false };
    float squishTimer{ 0.f };

public:
    Goomba(const sf::Vector2f& pos = { 0.f, 0.f });
    ~Goomba() override = default;

    EnemyContactOutcome handlePlayerContact(PlayerManager& player, int collisionSide, float horizontalDirection = 0.f) override;
    
    void onStomped() override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
