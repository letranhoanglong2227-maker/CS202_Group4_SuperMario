#pragma once

#include "Entities/Base/LivingEntity.hpp"

class Enemy : public LivingEntity {
protected:
    bool facingRight{ false };
    int damage{ 1 };
    int pointsValue{ 100 };
    bool stomped{ false };

public:
    Enemy();
    virtual ~Enemy() = default;

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
};
