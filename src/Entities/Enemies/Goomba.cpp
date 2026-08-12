#include "Entities/Enemies/Goomba.hpp"

Goomba::Goomba(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(1);
    setDamage(1);
    setPointsValue(100);
    setSpeed(50.f);

    setPosition(pos);
    hitbox.setSize(sf::Vector2f(32.f, 32.f));
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(50.f, 400.f, 0.f);
}

void Goomba::onStomped() {
    if (isSquished) return;
    isSquished = true;
    setStomped(true);
    setDamage(0);
    hitbox.setSize(sf::Vector2f(32.f, 16.f));
}

void Goomba::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (isSquished) {
            animationComponent->play("squished", dt);
        } else {
            animationComponent->play("walk", dt);
        }
    }
}

void Goomba::update(float dt) {
    if (dead) return;

    if (isSquished) {
        squishTimer += dt;
        if (squishTimer >= 0.5f) {
            setDead(true);
        }
        return;
    }

    float dirX = facingRight ? 1.f : -1.f;
    move(dirX, 0.f, dt);
    LivingEntity::update(dt);
}
