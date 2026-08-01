#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Components/MovementComponent.hpp"
#include <vector>

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
    
    // Di chuyển và xử lý va chạm trục X
    void moveX(sf::FloatRect& hitbox, MovementComponent& movement, const std::vector<sf::FloatRect>& blocks, float dt);
    
    // Di chuyển và xử lý va chạm trục Y (kết hợp nhảy, rớt)
    void moveY(sf::FloatRect& hitbox, MovementComponent& movement, const std::vector<sf::FloatRect>& blocks, float dt, CollisionInfo& info);
    
    // Kiểm tra an toàn trước khi lớn lên (thay đổi Hitbox)
    bool canGrow(const sf::FloatRect& currentHitbox, const sf::Vector2f& newSize, const std::vector<sf::FloatRect>& blocks);

private:
    float gravity;
};
