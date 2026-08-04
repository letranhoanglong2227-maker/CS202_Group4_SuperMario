#include "Physics/PhysicsEngine.hpp"
#include <algorithm>

PhysicsEngine::PhysicsEngine(float gravityY) : gravity(gravityY) {}

void PhysicsEngine::applyGravity(MovementComponent& movementComponent, float dt) {
    if (dt <= 0.f) return;
    // Giới hạn delta time để tránh hiện tượng rơi xuyên tường (tunneling) khi sụt giảm FPS
    float clampedDt = std::min(dt, 0.1f);
    movementComponent.applyForce({0.f, gravity * clampedDt});
}

void PhysicsEngine::moveX(sf::FloatRect& hitbox, MovementComponent& movement, const std::vector<sf::FloatRect>& blocks, float dt) {
    hitbox.position.x += movement.getVelocity().x * dt;
    for (const auto& block : blocks) {
        if (auto intersect = hitbox.findIntersection(block)) {
            // Đang đi sang phải
            if (movement.getVelocity().x > 0.f) {
                hitbox.position.x = block.position.x - hitbox.size.x;
            } 
            // Đang đi sang trái
            else if (movement.getVelocity().x < 0.f) {
                hitbox.position.x = block.position.x + block.size.x;
            }
            movement.setVelocity(0.f, movement.getVelocity().y);
        }
    }
}

void PhysicsEngine::moveY(sf::FloatRect& hitbox, MovementComponent& movement, const std::vector<sf::FloatRect>& blocks, float dt, CollisionInfo& info) {
    hitbox.position.y += movement.getVelocity().y * dt;
    info.grounded = false; // Reset grounded state

    for (const auto& block : blocks) {
        if (auto intersect = hitbox.findIntersection(block)) {
            // Đang rơi xuống (chạm đất)
            if (movement.getVelocity().y > 0.f) {
                hitbox.position.y = block.position.y - hitbox.size.y;
                info.grounded = true;
            } 
            // Đang nhảy lên (đụng trần)
            else if (movement.getVelocity().y < 0.f) {
                hitbox.position.y = block.position.y + block.size.y;
                info.ceilHit = true;
            }
            movement.setVelocity(movement.getVelocity().x, 0.f);
        }
    }
}

bool PhysicsEngine::canGrow(const sf::FloatRect& currentHitbox, const sf::Vector2f& newSize, const std::vector<sf::FloatRect>& blocks) {
    sf::FloatRect projectedHitbox = currentHitbox;
    projectedHitbox.position.y -= (newSize.y - currentHitbox.size.y);
    projectedHitbox.size = newSize;

    for (const auto& block : blocks) {
        if (projectedHitbox.findIntersection(block)) {
            return false;
        }
    }
    return true;
}
