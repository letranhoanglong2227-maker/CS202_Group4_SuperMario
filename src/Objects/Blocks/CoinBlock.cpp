#include "Objects/Blocks/CoinBlock.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Objects/Items/Coin.hpp"
#include <algorithm>
#include <cmath>
#include <utility>

CoinBlock::CoinBlock(int count, SpawnCallback spawnCallback,
                     RewardCallback rewardCallback)
    : coinCount(count), 
      isBouncing(false), 
      bounceTimer(0.f),
      bounceDuration(0.2f),
      maxBounceHeight(10.f),
      onSpawnItem(std::move(spawnCallback)),
      onReward(std::move(rewardCallback))
{
    if (count <= 0) {
        throw std::invalid_argument("CoinBlock must be initialized with at least 1 coin.");
    }

    name = "CoinBlock";
    exist = true;
    
    // Store original position (assumes position is set right after creation in the Level parser)
    // In update(), we'll check if originalY is uninitialized and set it, or you can set it when adding to the level.
    originalY = -1.f; 

    // Initialize textures and add animations directly in the constructor
    initSpritesSheet();

    animationComponent.addAnimation("Active", {spritesSheet["CoinBlock1"], spritesSheet["CoinBlock2"], spritesSheet["CoinBlock3"]});
    animationComponent.addAnimation("Empty", {spritesSheet["EmptyBlock"]});
}

void CoinBlock::initSpritesSheet() {
    spritesSheet["CoinBlock1"] = sf::IntRect({1, 52}, {16, 16});
    spritesSheet["CoinBlock2"] = sf::IntRect({18, 52}, {16, 16});
    spritesSheet["CoinBlock3"] = sf::IntRect({35, 52}, {16, 16});
    spritesSheet["EmptyBlock"] = sf::IntRect({1, 69}, {16, 16});
}

// ---> Note
void CoinBlock::reactToCollision(int collidedSide) {
    if (collidedSide == COLLISION_BOTTOM) {
        if (coinCount > 0 && !isBouncing) {
            // Trigger bounce
            isBouncing = true;
            bounceTimer = 0.f;
            
            // Spawn Coin
            if (onSpawnItem) {
                onSpawnItem(std::make_unique<Coin>(
                    sf::Vector2f{this->position.x,
                                 this->position.y - MapFormat::TILE_SIZE},
                    true));
            }
            if (onReward) onReward(100, 1);

            coinCount--;
        }
    }
}

// ---> Note
void CoinBlock::update(float dt) {
    // Lazy initialize originalY when update is first called (after position is actually set by the Level Manager)
    if (originalY < 0.f) {
        originalY = position.y;
    }

    // Play appropriate animation (Make sure these keys match what you added in the constructor)
    if (coinCount > 0) {
        animationComponent.play("Active", dt);
    } else {
        animationComponent.play("Empty", dt);
    }

    // Handle bouncing logic
    if (isBouncing) {
        bounceTimer = std::min(
            bounceTimer + std::max(0.f, dt), bounceDuration);
        const float phase = bounceDuration > 0.f
            ? bounceTimer / bounceDuration
            : 1.f;
        position.y = originalY -
            std::sin(phase * 3.14159265f) * maxBounceHeight;
        if (bounceTimer >= bounceDuration) {
            position.y = originalY;
            isBouncing = false;
            bounceTimer = 0.f;
        }
        entitySprite.setPosition(position);
    }
}

bool CoinBlock::isBumpingUpward() const noexcept {
    return isBouncing && bounceTimer <= bounceDuration * 0.5f;
}
