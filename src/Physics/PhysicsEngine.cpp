#include "Physics/PhysicsEngine.hpp"
#include <algorithm>

PhysicsEngine::PhysicsEngine(float gravityY) : gravity(gravityY) {}

void PhysicsEngine::applyGravity(MovementComponent& movementComponent, float dt) {
    if (dt <= 0.f) return;
    // Giới hạn delta time để tránh hiện tượng rơi xuyên tường (tunneling) khi sụt giảm FPS
    float clampedDt = std::min(dt, 0.1f);
    movementComponent.applyForce({0.f, gravity * clampedDt});
}

CollisionInfo PhysicsEngine::resolveCollision(sf::FloatRect& entityHitbox, MovementComponent& movement, const sf::FloatRect& blockHitbox) {
    CollisionInfo info;
    
    if (auto intersectionOpt = entityHitbox.findIntersection(blockHitbox)) {
        sf::FloatRect intersection = *intersectionOpt;
        info.collided = true;
        
        // Giải quyết theo hướng có độ lún (penetration) nhỏ nhất
        if (intersection.size.x < intersection.size.y) {
            // Va chạm theo phương ngang (X)
            info.wallHit = true;
            // Xác định xem thực thể nằm bên trái hay bên phải của block để đẩy ra
            float pushDir = (entityHitbox.position.x + entityHitbox.size.x / 2.f < blockHitbox.position.x + blockHitbox.size.x / 2.f) ? -1.f : 1.f;
            entityHitbox.position.x += intersection.size.x * pushDir;
            // Dừng vận tốc ngang
            movement.setVelocity(0.f, movement.getVelocity().y);
        } else {
            // Va chạm theo phương dọc (Y)
            if (entityHitbox.position.y + entityHitbox.size.y / 2.f < blockHitbox.position.y + blockHitbox.size.y / 2.f) {
                // Va chạm từ phía trên block -> Đáp đất
                info.grounded = true;
                entityHitbox.position.y -= intersection.size.y;
                // Dừng vận tốc rơi
                movement.setVelocity(movement.getVelocity().x, 0.f);
            } else {
                // Va chạm từ phía dưới block -> Đụng trần
                info.ceilHit = true;
                entityHitbox.position.y += intersection.size.y;
                // Triệt tiêu vận tốc hướng lên
                movement.setVelocity(movement.getVelocity().x, 0.f);
            }
        }
    }
    
    return info;
}
