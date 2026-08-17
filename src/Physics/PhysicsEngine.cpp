#include "Physics/PhysicsEngine.hpp"
#include "Entities/Base/Character.hpp"
#include "Entities/Base/Enemy.hpp"
#include "Entities/Base/LivingEntity.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Objects/Blocks/Block.hpp"
#include "Objects/Blocks/MovingBlock.hpp"
#include "Objects/Environment/Trampoline.hpp"
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

std::vector<Block*> PhysicsEngine::queryNearbyBlocks(
    const sf::FloatRect& entityHitbox, const std::vector<Block*>& blocks,
    unsigned int paddingTiles) const {
    const float padding = static_cast<float>(paddingTiles) * MapFormat::TILE_SIZE;
    const sf::FloatRect queryArea(
        {entityHitbox.position.x - padding, entityHitbox.position.y - padding},
        {entityHitbox.size.x + 2.f * padding,
         entityHitbox.size.y + 2.f * padding});

    std::vector<Block*> nearby;
    nearby.reserve(blocks.size());
    for (Block* block : blocks) {
        if (block && queryArea.findIntersection(block->hitbox.getGlobalBounds())) {
            nearby.push_back(block);
        }
    }
    return nearby;
}

CollisionInfo PhysicsEngine::step(LivingEntity& entity,
                                  const std::vector<Block*>& blocks,
                                  float dt) {
    CollisionInfo info;
    MovementComponent* movement = entity.getMovementComponent();
    if (!movement || dt <= 0.f) return info;

    const float safeDt = std::min(dt, 0.05f);
    applyGravity(*movement, safeDt);

    sf::FloatRect bounds = entity.hitbox.getGlobalBounds();
    const std::vector<Block*> nearby = queryNearbyBlocks(bounds, blocks, 2);

    bounds.position.x += movement->getVelocity().x * safeDt;
    for (Block* block : nearby) {
        if (!block) continue;
        const sf::FloatRect blockBounds = block->hitbox.getGlobalBounds();
        if (!bounds.findIntersection(blockBounds)) continue;
        if (movement->getVelocity().x > 0.f) {
            bounds.position.x = blockBounds.position.x - bounds.size.x;
            block->reactToCollision(2);
        } else if (movement->getVelocity().x < 0.f) {
            bounds.position.x = blockBounds.position.x + blockBounds.size.x;
            block->reactToCollision(3);
        }
        movement->setVelocity(0.f, movement->getVelocity().y);
        info.collided = true;
        info.wallHit = true;
        if (auto* enemy = dynamic_cast<Enemy*>(&entity)) enemy->reverseDirection();
    }

    bounds.position.y += movement->getVelocity().y * safeDt;
    for (Block* block : nearby) {
        if (!block) continue;
        const sf::FloatRect blockBounds = block->hitbox.getGlobalBounds();
        if (!bounds.findIntersection(blockBounds)) continue;
        if (movement->getVelocity().y > 0.f) {
            bounds.position.y = blockBounds.position.y - bounds.size.y;
            movement->setVelocity(movement->getVelocity().x, 0.f);
            info.collided = true;
            info.grounded = true;
            block->reactToCollision(1);
            if (auto* trampoline = dynamic_cast<Trampoline*>(block)) {
                movement->setVelocity(movement->getVelocity().x,
                                      trampoline->getLaunchVelocity());
                info.grounded = false;
            } else if (auto* moving = dynamic_cast<MovingBlock*>(block)) {
                bounds.position += moving->getFrameDelta();
            }
        } else if (movement->getVelocity().y < 0.f) {
            bounds.position.y = blockBounds.position.y + blockBounds.size.y;
            movement->setVelocity(movement->getVelocity().x, 0.f);
            info.collided = true;
            info.ceilHit = true;
            block->reactToCollision(0);
        }
    }

    entity.hitbox.setPosition(bounds.position);
    entity.setPosition(bounds.position);
    if (auto* character = dynamic_cast<Character*>(&entity)) {
        character->setGrounded(info.grounded);
    }
    return info;
}
