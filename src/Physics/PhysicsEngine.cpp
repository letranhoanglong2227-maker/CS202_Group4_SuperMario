#include "Physics/PhysicsEngine.hpp"
#include "Entities/Base/Character.hpp"
#include "Entities/Base/Enemy.hpp"
#include "Entities/Base/LivingEntity.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Objects/Blocks/Block.hpp"
#include "Objects/Blocks/MovingBlock.hpp"
#include "Objects/Environment/Trampoline.hpp"
#include <algorithm>
#include <limits>
#include <utility>

PhysicsEngine::PhysicsEngine(float gravityY) : gravity(gravityY) {}

void PhysicsEngine::applyGravity(MovementComponent& movementComponent, float dt) {
    if (dt <= 0.f) return;
    // Giới hạn delta time để tránh hiện tượng rơi xuyên tường (tunneling) khi sụt giảm FPS
    float clampedDt = std::min(dt, 0.1f);
    movementComponent.applyForce({0.f, gravity * clampedDt});
}

bool PhysicsEngine::canGrow(const sf::FloatRect& currentHitbox,
                            const sf::Vector2f& newSize,
                            const std::vector<sf::FloatRect>& blocks) const {
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

AabbContactSide PhysicsEngine::classifyAabbContact(
    const sf::FloatRect& previousBounds,
    const sf::FloatRect& currentBounds,
    const sf::FloatRect& otherBounds) {
    const auto intersection = currentBounds.findIntersection(otherBounds);
    if (!intersection) return AabbContactSide::None;

    const sf::Vector2f delta =
        currentBounds.position - previousBounds.position;
    const float previousLeft = previousBounds.position.x;
    const float previousRight = previousLeft + previousBounds.size.x;
    const float previousTop = previousBounds.position.y;
    const float previousBottom = previousTop + previousBounds.size.y;
    const float otherLeft = otherBounds.position.x;
    const float otherRight = otherLeft + otherBounds.size.x;
    const float otherTop = otherBounds.position.y;
    const float otherBottom = otherTop + otherBounds.size.y;

    float xEntry = -1.f;
    AabbContactSide xSide = AabbContactSide::None;
    if (delta.x > 0.f && previousRight <= otherLeft) {
        xEntry = (otherLeft - previousRight) / delta.x;
        xSide = AabbContactSide::Right;
    } else if (delta.x < 0.f && previousLeft >= otherRight) {
        xEntry = (otherRight - previousLeft) / delta.x;
        xSide = AabbContactSide::Left;
    }

    float yEntry = -1.f;
    AabbContactSide ySide = AabbContactSide::None;
    if (delta.y > 0.f && previousBottom <= otherTop) {
        yEntry = (otherTop - previousBottom) / delta.y;
        ySide = AabbContactSide::Bottom;
    } else if (delta.y < 0.f && previousTop >= otherBottom) {
        yEntry = (otherBottom - previousTop) / delta.y;
        ySide = AabbContactSide::Top;
    }

    const bool validXEntry = xEntry >= 0.f && xEntry <= 1.f;
    const bool validYEntry = yEntry >= 0.f && yEntry <= 1.f;
    if (validXEntry || validYEntry) {
        if (!validYEntry || (validXEntry && xEntry >= yEntry)) return xSide;
        return ySide;
    }

    // Already-overlapping or stationary fallback: use minimum penetration,
    // matching Group5's useful AABB idea without inheriting its ownership or
    // collision-orchestration design. Horizontal wins a tie deliberately.
    if (intersection->size.x <= intersection->size.y) {
        const float currentCenterX =
            currentBounds.position.x + currentBounds.size.x * 0.5f;
        const float otherCenterX = otherLeft + otherBounds.size.x * 0.5f;
        return currentCenterX < otherCenterX ? AabbContactSide::Right
                                            : AabbContactSide::Left;
    }

    const float currentCenterY =
        currentBounds.position.y + currentBounds.size.y * 0.5f;
    const float otherCenterY = otherTop + otherBounds.size.y * 0.5f;
    return currentCenterY < otherCenterY ? AabbContactSide::Bottom
                                        : AabbContactSide::Top;
}

bool PhysicsEngine::sweptAabbIntersects(
    const sf::FloatRect& previousBounds,
    const sf::FloatRect& currentBounds,
    const sf::FloatRect& otherBounds) noexcept {
    if (previousBounds.findIntersection(otherBounds) ||
        currentBounds.findIntersection(otherBounds)) {
        return true;
    }

    const sf::Vector2f delta =
        currentBounds.position - previousBounds.position;
    const float infinity = std::numeric_limits<float>::infinity();

    const auto axisTimes = [infinity](float movingMin, float movingMax,
                                      float targetMin, float targetMax,
                                      float movement) {
        if (movement > 0.f) {
            return std::pair{(targetMin - movingMax) / movement,
                             (targetMax - movingMin) / movement};
        }
        if (movement < 0.f) {
            return std::pair{(targetMax - movingMin) / movement,
                             (targetMin - movingMax) / movement};
        }
        if (movingMax <= targetMin || movingMin >= targetMax) {
            return std::pair{infinity, -infinity};
        }
        return std::pair{-infinity, infinity};
    };

    const auto [xEntry, xExit] = axisTimes(
        previousBounds.position.x,
        previousBounds.position.x + previousBounds.size.x,
        otherBounds.position.x, otherBounds.position.x + otherBounds.size.x,
        delta.x);
    const auto [yEntry, yExit] = axisTimes(
        previousBounds.position.y,
        previousBounds.position.y + previousBounds.size.y,
        otherBounds.position.y, otherBounds.position.y + otherBounds.size.y,
        delta.y);

    const float entryTime = std::max(xEntry, yEntry);
    const float exitTime = std::min(xExit, yExit);
    return entryTime <= exitTime && entryTime >= 0.f && entryTime <= 1.f;
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
        if (block && block->isExist() &&
            queryArea.findIntersection(block->hitbox.getGlobalBounds())) {
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
        if (!block || !block->isExist()) continue;
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
        if (!block || !block->isExist()) continue;
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

void PhysicsEngine::enforceHorizontalBounds(
    LivingEntity& entity, const sf::FloatRect& worldBounds) const {
    if (worldBounds.size.x <= 0.f) return;

    const sf::FloatRect bounds = entity.hitbox.getGlobalBounds();
    const float minX = worldBounds.position.x;
    const float maxX =
        minX + std::max(0.f, worldBounds.size.x - bounds.size.x);
    const float resolvedX = std::clamp(bounds.position.x, minX, maxX);

    if (MovementComponent* movement = entity.getMovementComponent()) {
        const sf::Vector2f velocity = movement->getVelocity();
        if ((resolvedX <= minX && velocity.x < 0.f) ||
            (resolvedX >= maxX && velocity.x > 0.f)) {
            movement->setVelocity(0.f, velocity.y);
        }
    }

    if (resolvedX != bounds.position.x) {
        entity.setPosition({resolvedX, bounds.position.y});
    }
}
