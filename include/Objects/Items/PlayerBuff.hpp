#pragma once

#include <string>

class PlayerManager;

class PlayerBuff {
protected:
    std::string buffType;
    float duration{ 0.f };
    float timer{ 0.f };
    bool expired{ false };

public:
    PlayerBuff(const std::string& type = "GenericBuff", float dur = 5.f);
    virtual ~PlayerBuff() = default;

    const std::string& getType() const;
    bool isExpired() const;

    virtual void apply(PlayerManager* player);
    virtual void update(float dt);
    virtual void remove(PlayerManager* player);
};
