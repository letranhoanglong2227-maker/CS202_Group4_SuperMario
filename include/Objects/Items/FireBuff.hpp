#pragma once

#include "Objects/Items/PlayerBuff.hpp"

class FireBuff : public PlayerBuff {
private:
    int fireballCount{ 10 };

public:
    FireBuff(float dur = 0.f); // 0.f means permanent until damaged
    ~FireBuff() override = default;

    void apply(PlayerManager* player) override;
    void remove(PlayerManager* player) override;
};
