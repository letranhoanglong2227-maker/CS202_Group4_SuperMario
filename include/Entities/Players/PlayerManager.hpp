#pragma once

#include "Entities/Base/Character.hpp"
#include "Objects/Items/PlayerBuff.hpp"
#include <vector>
#include <memory>

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

    std::vector<std::unique_ptr<PlayerBuff>> buffs;

public:
    PlayerManager(int pId = 1, const std::string& name = "Player");
    virtual ~PlayerManager() = default;

    int getPlayerId() const;
    void setPlayerId(int pId);

    bool isBig() const;
    void setBig(bool big);

    bool isFire() const;
    void setFire(bool fire);

    bool isImmortal() const;
    void setImmortal(bool val, float duration = 2.0f);

    void addBuff(std::unique_ptr<PlayerBuff> buff);
    void removeBuff(const std::string& type);

    virtual void handleInput(float dt);
    virtual void updateHitboxSize();

    void takeDamage(int amount) override;
    void update(float dt) override;
};
