#include "Objects/Blocks/CoinBlock.hpp"
#include "Objects/Items/Coin.hpp"

CoinBlock::CoinBlock(int count, std::function<void(GameObject*)> spawnCallback)
    : coinCount(count), 
      isBouncing(false), 
      bounceVelocity(0.f), 
      bounceGravity(1500.f), 
      maxBounceHeight(10.f),
      onSpawnItem(spawnCallback) 
{
    if (count <= 0) {
        throw std::invalid_argument("CoinBlock must be initialized with at least 1 coin.");
    }

    name = "CoinBlock";
    
    // Store original position (assumes position is set right after creation in the Level parser)
    // In update(), we'll check if originalY is uninitialized and set it, or you can set it when adding to the level.
    originalY = -1.f; 

    // Initialize textures and add animations directly in the constructor
    initSpritesSheet();

    // TODO: USER TASK 
    animationComponent.addAnimation("Active", {spritesSheet["CoinBlock1"], spritesSheet["CoinBlock2"], spritesSheet["CoinBlock3"]});
    animationComponent.addAnimation("Empty", {spritesSheet["EmptyBlock"]});
}

void CoinBlock::initSpritesSheet() {
    // TODO: USER TASK - Put your exact IntRect coordinates here!
    spritesSheet["CoinBlock1"] = sf::IntRect({2, 96}, {16, 16});
    spritesSheet["CoinBlock2"] = sf::IntRect({19, 96}, {16, 16});
    spritesSheet["CoinBlock3"] = sf::IntRect({36, 96}, {16, 16});
    spritesSheet["EmptyBlock"] = sf::IntRect({2, 113}, {16, 16});
}

// ---> Note
void CoinBlock::reactToCollision(int collidedSide) {
    if (collidedSide == COLLISION_BOTTOM) {
        if (coinCount > 0 && !isBouncing) {
            // Trigger bounce
            isBouncing = true;
            bounceVelocity = -100.f; // Move up
            
            // Spawn Coin
            if (onSpawnItem) {
                Coin* newCoin = new Coin({this->position.x, this->position.y - 32.f}, true); 
                onSpawnItem(newCoin);
            }

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
        position.y += bounceVelocity * dt;
        bounceVelocity += bounceGravity * dt; // Gravity pulls it back down

        // If it falls back down to its original position
        if (position.y >= originalY) {
            position.y = originalY;
            isBouncing = false;
            bounceVelocity = 0.f;
        }
        
        // Update sprite position
        entitySprite.setPosition(position);
    }
}
