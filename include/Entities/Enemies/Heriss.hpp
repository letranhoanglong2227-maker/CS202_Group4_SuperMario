#pragma once

#include "Entities/Base/Enemy.hpp"

class Heriss : public Enemy {
public:
    Heriss(const sf::Vector2f& pos = { 0.f, 0.f });
    ~Heriss() override = default;

    void onStomped() override;
    EnemyContactOutcome handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection = 0.f) override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
