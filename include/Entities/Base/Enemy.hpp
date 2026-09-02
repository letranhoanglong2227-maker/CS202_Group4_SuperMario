#include <optional>
#include "Objects/Items/ProjectileSpawnRequest.hpp"
#pragma once

#include "Core/AssetLocator.hpp"
#include "Entities/Base/LivingEntity.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Entities/Base/EnemyContactOutcome.hpp"
#include <iostream>

class Enemy : public LivingEntity {
protected:
    bool facingRight{ false };
    int damage{ 1 };
    int pointsValue{ 100 };
    bool stomped{ false };

public:
    Enemy();
    virtual ~Enemy() = default;

    virtual std::optional<ProjectileSpawnRequest> consumePendingProjectile();

    virtual EnemyContactOutcome handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection = 0.f);

    bool isFacingRight() const;
    void setFacingRight(bool right);

    int getDamage() const;
    void setDamage(int d);

    int getPointsValue() const;
    void setPointsValue(int pts);

    bool isStomped() const;
    void setStomped(bool s);

    virtual void reverseDirection();
    virtual void onStomped();
    virtual void reactToCollision(int collidedSide);
};

class TextureEnemyManager {
private:
    inline static sf::Texture enemyTexture;
public:
    static bool setupTexture() {
        const auto path = AssetLocator::find("assets/textures/Enemies.png");
        const bool loadOk = path && enemyTexture.loadFromFile(*path);
        if (!loadOk)
            std::cerr << AssetLocator::missingMessage(
                             "assets/textures/Enemies.png")
                      << '\n';
        return loadOk;
    }

    static sf::Texture& getEnemyTexture() {
        return enemyTexture;
    }
};
