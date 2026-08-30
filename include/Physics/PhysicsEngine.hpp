#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Components/MovementComponent.hpp"
#include <vector>

class Block;
class LivingEntity;

enum class AabbContactSide {
    None,
    Top,
    Bottom,
    Left,
    Right
};

struct CollisionInfo {
    bool collided = false;
    bool grounded = false;  // Va chạm từ trên đỉnh block (đáp đất)
    bool ceilHit = false;   // Va chạm từ dưới đáy block (đụng trần)
    bool wallHit = false;   // Va chạm từ hai bên hông
};

class PhysicsEngine {
public:
    PhysicsEngine(float gravityY = 980.f);

    // Kiểm tra an toàn trước khi lớn lên (thay đổi Hitbox)
    bool canGrow(const sf::FloatRect& currentHitbox, const sf::Vector2f& newSize,
                 const std::vector<sf::FloatRect>& blocks) const;

    // Classifies which side of the moving AABB contacted the other AABB.
    // Previous bounds disambiguate corner contacts; an ambiguous tie resolves
    // horizontally so a corner cannot be mistaken for a stomp.
    static AabbContactSide classifyAabbContact(
        const sf::FloatRect& previousBounds,
        const sf::FloatRect& currentBounds,
        const sf::FloatRect& otherBounds);
    static bool sweptAabbIntersects(
        const sf::FloatRect& previousBounds,
        const sf::FloatRect& currentBounds,
        const sf::FloatRect& otherBounds) noexcept;

    CollisionInfo step(LivingEntity& entity, const std::vector<Block*>& blocks,
                       float dt);
    void enforceHorizontalBounds(LivingEntity& entity,
                                 const sf::FloatRect& worldBounds) const;
    std::vector<Block*> queryNearbyBlocks(const sf::FloatRect& entityHitbox,
                                          const std::vector<Block*>& blocks,
                                          unsigned int paddingTiles = 1) const;

private:
    void applyGravity(MovementComponent& movementComponent, float dt);

    float gravity;
};
