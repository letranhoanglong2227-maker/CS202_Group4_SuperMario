#pragma once

#include "Entities/Base/Enemy.hpp"

#include "Objects/Items/ProjectileSpawnRequest.hpp"
#include <optional>

class PeteyPiranha : public Enemy {
private:
    float emergeTimer{ 0.f };
    bool emerged{ false };
    float baseY{ 0.f };
    std::optional<ProjectileSpawnRequest> pendingProjectile;
    bool hasShot{ false };

public:
    PeteyPiranha(const sf::Vector2f& pos = { 0.f, 0.f });
    ~PeteyPiranha() override = default;

    bool isEmerged() const;
    void emerge();
    void retract();

    EnemyContactOutcome handlePlayerContact(PlayerManager& player, int collisionSide, float horizontalDirection = 0.f) override;

    bool hasPendingProjectile() const;
    std::optional<ProjectileSpawnRequest> consumePendingProjectile();

    void takeDamage(int amount) override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
