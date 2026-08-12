#pragma once

#include "Entities/Enemies/Koopa.hpp"

class FlyingKoopa : public Koopa {
private:
    bool hasWings{ true };
    float flyTimer{ 0.f };
    float initialY{ 0.f };

public:
    FlyingKoopa(const sf::Vector2f& pos = { 0.f, 0.f });
    ~FlyingKoopa() override = default;

    bool isFlying() const;
    void onStomped() override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
