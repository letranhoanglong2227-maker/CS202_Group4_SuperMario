#pragma once

#include "Entities/Base/Enemy.hpp"

#include "Objects/Items/ProjectileSpawnRequest.hpp"
#include <optional>

class Bowser : public Enemy {
private:
    float attackTimer{ 0.f };
    bool breathingFire{ false };
    bool jumpingSlam{ false };
    std::optional<ProjectileSpawnRequest> pendingProjectile;

public:
    Bowser(const sf::Vector2f& pos = { 0.f, 0.f });
    ~Bowser() override = default;

    bool isBreathingFire() const;
    void breatheFire();
    void jumpSlam();

    EnemyContactOutcome handlePlayerContact(PlayerManager& player, int collisionSide, float horizontalDirection = 0.f) override;

    bool hasPendingProjectile() const;
    std::optional<ProjectileSpawnRequest> consumePendingProjectile();

    void takeDamage(int amount) override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
