#pragma once

#include "Entities/Base/Character.hpp"
#include "Objects/Items/PlayerBuff.hpp"
#include "Objects/Items/ProjectileSpawnRequest.hpp"
#include <vector>
#include <memory>
#include <optional>

class PlayerManager : public Character {
protected:
    int playerId{ 1 }; // 1: Player 1 (Mario), 2: Player 2 (Luigi)
    bool is_big{ false };
    bool is_fire{ false };
    bool buffed_fire{ false };

    bool immortal{ false };
    bool isFlashing{ false };
    float flashTimer{ 0.f };
    float invincibilityDuration{ 2.0f };

    bool isTransforming{ false };
    float transformationTimer{ 0.f };
    bool horizontalControlsInverted{ false };
    bool jumpHeld{ false };
    bool running{ false };
    bool crouching{ false };
    int maxAirJumps{ 1 };
    int airJumpsRemaining{ 1 };
    
    float shootTimer{ 0.f };
    float shootCooldown{ 0.3f };

    std::vector<std::unique_ptr<PlayerBuff>> buffs;

    void syncVisualToHitbox() override;

public:
    PlayerManager(int pId = 1, const std::string& name = "Player");
    virtual ~PlayerManager() = default;

    int getPlayerId() const;
    void setPlayerId(int pId);

    bool isBig() const;
    bool setBig(bool big, bool canGrow = true);

    bool isFire() const;
    bool setFire(bool fire, bool canGrow = true);

    void resetForRespawn(const sf::Vector2f& spawnPosition = {0.f, 0.f});

    bool canShoot() const;
    std::optional<ProjectileSpawnRequest> shoot(float direction);

    bool isImmortal() const;
    void setImmortal(bool val, float duration = 2.0f);

    bool areHorizontalControlsInverted() const noexcept;
    void setHorizontalControlsInverted(bool inverted) noexcept;

    void addBuff(std::unique_ptr<PlayerBuff> buff);
    void removeBuff(const std::string& type);

    virtual void handleInput(float dt);
    virtual void updateHitboxSize();
    bool canJump() const noexcept;
    void jump() override;

    void takeDamage(int amount) override;
    void update(float dt) override;
};
