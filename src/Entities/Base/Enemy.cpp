#include "Entities/Base/Enemy.hpp"

Enemy::Enemy()
    : LivingEntity() {
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
