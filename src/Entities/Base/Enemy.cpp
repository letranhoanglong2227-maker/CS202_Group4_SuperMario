#include "Entities/Base/Enemy.hpp"

Enemy::Enemy() : LivingEntity("Enemy") {
    animationComponent = std::make_unique<AnimationComponent>(
        entitySprite, 
        TextureEnemyManager::getEnemyTexture(), 
        0.15f
    );
}

EnemyContactOutcome Enemy::handlePlayerContact(PlayerManager& player, int collisionSide, float horizontalDirection) {
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    
    // Default implementation:
    // If stomped from top (enemy's top) or player's bottom
    if (collisionSide == 1 || collisionSide == 0) {
        onStomped();
        return EnemyContactOutcome{EnemyContactResult::EnemyStomped, pointsValue, -500.f, true};
    }
    
    // Side collision
    player.takeDamage(damage);
    return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
}

bool Enemy::isFacingRight() const {
    return facingRight;
}

void Enemy::setFacingRight(bool right) {
    facingRight = right;
}

int Enemy::getDamage() const {
    return damage;
}

void Enemy::setDamage(int d) {
    damage = d;
}

int Enemy::getPointsValue() const {
    return pointsValue;
}

void Enemy::setPointsValue(int pts) {
    pointsValue = pts;
}

bool Enemy::isStomped() const {
    return stomped;
}

void Enemy::setStomped(bool s) {
    stomped = s;
}

void Enemy::reverseDirection() {
    facingRight = !facingRight;
}

void Enemy::onStomped() {
    stomped = true;
    takeDamage(health);
}

void Enemy::reactToCollision(int collidedSide) {
    (void)collidedSide;
}
