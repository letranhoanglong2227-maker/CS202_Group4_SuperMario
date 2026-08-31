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
    syncVisualToHitbox();
}

void LivingEntity::syncVisualToHitbox() {
    const sf::Vector2i frameSize = entitySprite.getTextureRect().size;
    const sf::Vector2f hitboxSize = hitbox.getSize();
    if (frameSize.x > 0 && frameSize.y > 0 &&
        hitboxSize.x > 0.f && hitboxSize.y > 0.f) {
        const bool facingLeft = entitySprite.getScale().x < 0.f;
        const float scale = hitboxSize.x / static_cast<float>(frameSize.x);
        entitySprite.setScale({
            (facingLeft ? -1.f : 1.f) * scale, scale});
        entitySprite.setOrigin(
            facingLeft ? sf::Vector2f{static_cast<float>(frameSize.x), 0.f}
                       : sf::Vector2f{});
        const sf::Vector2f hitboxPosition = hitbox.getPosition();
        entitySprite.setPosition({
            hitboxPosition.x,
            hitboxPosition.y + hitboxSize.y -
                static_cast<float>(frameSize.y) * scale});
    } else {
        entitySprite.setPosition(hitbox.getPosition());
    }
    position = hitbox.getPosition();
}

void LivingEntity::render(sf::RenderTarget* target) {
    if (target) {
        syncVisualToHitbox();
        target->draw(entitySprite);
    }
}
