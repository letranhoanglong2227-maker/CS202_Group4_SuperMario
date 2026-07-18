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

void MovementComponent::applyForce(const sf::Vector2f& force) {
    velocity += force;
}

void MovementComponent::move(float dirX, float dirY, float dt) {
    if (dt <= 0.f) return;
    velocity.x += dirX * acceleration * dt;
    velocity.y += dirY * acceleration * dt;
}

void MovementComponent::update(float dt) {
    if (dt <= 0.f) return;
    // Áp dụng ma sát (friction) theo trục X nếu không có lực đẩy
    if (velocity.x > 0.f) {
        velocity.x -= friction * dt;
        if (velocity.x < 0.f) velocity.x = 0.f;
    } else if (velocity.x < 0.f) {
        velocity.x += friction * dt;
        if (velocity.x > 0.f) velocity.x = 0.f;
    }

    // Giới hạn vận tốc trục X
    if (velocity.x > maxVelocity) velocity.x = maxVelocity;
    if (velocity.x < -maxVelocity) velocity.x = -maxVelocity;
    
    // Giới hạn vận tốc rơi (terminal velocity)
    float terminalVelocity = maxVelocity * 2.f; 
    if (velocity.y > terminalVelocity) velocity.y = terminalVelocity;
}
