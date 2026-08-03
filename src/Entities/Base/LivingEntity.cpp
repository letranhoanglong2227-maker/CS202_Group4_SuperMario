#include "Entities/Base/LivingEntity.hpp"

LivingEntity::LivingEntity()
    : GameObject() {
}

int LivingEntity::getHealth() const {
    return health;
}

void LivingEntity::setHealth(int h) {
    health = h;
    if (health <= 0) {
        dead = true;
    }
}

bool LivingEntity::isDead() const {
    return dead;
}

void LivingEntity::setDead(bool d) {
    dead = d;
}

float LivingEntity::getSpeed() const {
    return speed;
}

void LivingEntity::setSpeed(float s) {
    speed = s;
}

MovementComponent* LivingEntity::getMovementComponent() const {
    return movementComponent.get();
}

AnimationComponent* LivingEntity::getAnimationComponent() const {
    return animationComponent.get();
}

void LivingEntity::takeDamage(int amount) {
    if (dead) return;
    health -= amount;
    if (health <= 0) {
        health = 0;
        dead = true;
    }
}

void LivingEntity::move(float dirX, float dirY, float dt) {
    if (movementComponent) {
        movementComponent->move(dirX, dirY, dt);
    }
}

void LivingEntity::updateVelocity(float dt) {
    if (movementComponent) {
        movementComponent->update(dt);
    }
}

void LivingEntity::updateAnimation(float dt) {
    (void)dt;
}

void LivingEntity::update(float dt) {
    updateVelocity(dt);
    updateAnimation(dt);
    entitySprite.setPosition(hitbox.getPosition());
    position = hitbox.getPosition();
}

void LivingEntity::render(sf::RenderTarget* target) {
    if (target) {
        target->draw(entitySprite);
    }
}
