#pragma once

#include "Core/GameObject.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/AnimationComponent.hpp"
#include <memory>

class LivingEntity : public GameObject {
protected:
    int health{ 1 };
    float speed{ 100.f };
    bool dead{ false };

    std::unique_ptr<MovementComponent> movementComponent;
    std::unique_ptr<AnimationComponent> animationComponent;

public:
    LivingEntity();
    virtual ~LivingEntity() = default;

    int getHealth() const;
    void setHealth(int h);
    bool isDead() const;
    void setDead(bool d);

    float getSpeed() const;
    void setSpeed(float s);

    MovementComponent* getMovementComponent() const;
    AnimationComponent* getAnimationComponent() const;

    virtual void takeDamage(int amount);
    virtual void move(float dirX, float dirY, float dt);
    virtual void updateVelocity(float dt);
    virtual void updateAnimation(float dt);

    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
};
