#include "Components/MovementComponent.hpp"
#include <cmath>

MovementComponent::MovementComponent(float maxVelocity, float acceleration, float friction)
    : velocity{0.f, 0.f}, maxVelocity(maxVelocity), acceleration(acceleration), friction(friction) {
}

const sf::Vector2f& MovementComponent::getVelocity() const {
    return velocity;
}

void MovementComponent::setVelocity(const sf::Vector2f& vel) {
    velocity = vel;
}

void MovementComponent::setVelocity(float x, float y) {
    velocity.x = x;
    velocity.y = y;
}

void MovementComponent::setMaxVelocity(float value) {
    if (value >= 0.f) maxVelocity = value;
}

void MovementComponent::applyForce(const sf::Vector2f& force) {
    velocity += force;
}

void MovementComponent::move(float dirX, float dirY, float dt) {
    if (dt <= 0.f) return;
    horizontalInputApplied = horizontalInputApplied || dirX != 0.f;
    velocity.x += dirX * acceleration * dt;
    velocity.y += dirY * acceleration * dt;
}

void MovementComponent::update(float dt) {
    if (dt <= 0.f) return;
    // Áp dụng ma sát (friction) theo trục X nếu không có lực đẩy
    if (!horizontalInputApplied) {
        if (velocity.x > 0.f) {
            velocity.x -= friction * dt;
            if (velocity.x < 0.f) velocity.x = 0.f;
        } else if (velocity.x < 0.f) {
            velocity.x += friction * dt;
            if (velocity.x > 0.f) velocity.x = 0.f;
        }
    }
    horizontalInputApplied = false;

    // Giới hạn vận tốc trục X
    if (velocity.x > maxVelocity) velocity.x = maxVelocity;
    if (velocity.x < -maxVelocity) velocity.x = -maxVelocity;
    
    // Match the reference game's world-scale fall speed. Tying this to the
    // horizontal cap made slow enemies and the player float unnaturally.
    constexpr float terminalVelocity = 1400.f;
    if (velocity.y > terminalVelocity) velocity.y = terminalVelocity;
}
