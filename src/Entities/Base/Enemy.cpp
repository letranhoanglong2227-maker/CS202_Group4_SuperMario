#include "Entities/Base/Enemy.hpp"

#include <cmath>

Enemy::Enemy() : LivingEntity() {
    animationComponent = std::make_unique<AnimationComponent>(
        entitySprite,
        TextureEnemyManager::getEnemyTexture(),
        0.2f
    );
}

EnemyContactOutcome Enemy::handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection) {
    (void)horizontalDirection;
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};

    if (kind == PlayerEnemyContactKind::Stomp) {
        onStomped();
        return EnemyContactOutcome{EnemyContactResult::EnemyStomped, pointsValue, -500.f, true};
    }

    // Side collision
    if (player.isImmortal()) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    player.takeDamage(damage);
    return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
}

bool Enemy::isFacingRight() const {
    return facingRight;
}

void Enemy::setFacingRight(bool right) {
    facingRight = right;
    if (movementComponent) {
        const sf::Vector2f velocity = movementComponent->getVelocity();
        if (velocity.x != 0.f) {
            movementComponent->setVelocity(
                right ? std::abs(velocity.x) : -std::abs(velocity.x),
                velocity.y);
        }
    }
    const sf::Vector2f scale = entitySprite.getScale();
    entitySprite.setScale({right ? std::abs(scale.x) : -std::abs(scale.x),
                           scale.y});
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
    setFacingRight(!facingRight);
}

void Enemy::onStomped() {
    stomped = true;
    takeDamage(health);
}

void Enemy::reactToCollision(int collidedSide) {
    (void)collidedSide;
}

std::optional<ProjectileSpawnRequest> Enemy::consumePendingProjectile() {
    return std::nullopt;
}
