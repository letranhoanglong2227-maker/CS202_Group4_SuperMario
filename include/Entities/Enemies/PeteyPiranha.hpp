#pragma once

#include "Entities/Base/Enemy.hpp"

class PeteyPiranha : public Enemy {
private:
    float emergeTimer{ 0.f };
    bool emerged{ false };
    std::optional<ProjectileSpawnRequest> pendingProjectile;

public:
    PeteyPiranha(const sf::Vector2f& pos = { 0.f, 0.f });
    ~PeteyPiranha() override = default;

    bool isEmerged() const;
    void emerge();
    void retract();

    EnemyContactOutcome handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection = 0.f) override;

    bool hasPendingProjectile() const;
    std::optional<ProjectileSpawnRequest> consumePendingProjectile() override;

    void takeDamage(int amount) override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
