#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Components/MovementComponent.hpp"

struct CollisionInfo {
    bool collided = false;
    bool grounded = false;  // Va chạm từ trên đỉnh block (đáp đất)
    bool ceilHit = false;   // Va chạm từ dưới đáy block (đụng trần)
    bool wallHit = false;   // Va chạm từ hai bên hông
};

class PhysicsEngine {
public:
    PhysicsEngine(float gravityY = 980.f);

    void applyGravity(MovementComponent& movementComponent, float dt);
    
    // Giải quyết va chạm AABB giữa thực thể (hitbox) và một block tĩnh (blockHitbox)
    CollisionInfo resolveCollision(sf::FloatRect& entityHitbox, MovementComponent& movement, const sf::FloatRect& blockHitbox);

private:
    float gravity;
};
