#include "Entities/Enemies/Heriss.hpp"

Heriss::Heriss(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(1);
    setDamage(1);
    setPointsValue(200);
    setSpeed(55.f);

    setPosition(pos);
    hitbox.setSize(sf::Vector2f(32.f, 32.f));
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(55.f, 500.f, 0.f);
}

void Heriss::onStomped() {
    // Spiked top protection: Stomping Heriss does not kill it
}

void Heriss::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        animationComponent->play("walk", dt);
    }
}

void Heriss::update(float dt) {
    if (dead) return;

    float dirX = facingRight ? 1.f : -1.f;
    move(dirX, 0.f, dt);
    LivingEntity::update(dt);
}
