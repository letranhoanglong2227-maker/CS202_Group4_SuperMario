#include "Objects/Blocks/MushroomBlock.hpp"
#include "Objects/Items/Mushroom.hpp"

// We declare COLLISION_BOTTOM here if not already available in a global header
#ifndef COLLISION_BOTTOM_DEF
#define COLLISION_BOTTOM_DEF
const int COLLISION_BOTTOM = 0; 
#endif

MushroomBlock::MushroomBlock(std::function<void(GameObject*)> spawnCallback)
    : isEmpty(false), 
      isBouncing(false), 
      bounceVelocity(0.f), 
      bounceGravity(1500.f), 
      maxBounceHeight(10.f),
      onSpawnItem(spawnCallback) 
{
    name = "MushroomBlock";
    originalY = -1.f; 

    // Initialize textures and add animations directly in the constructor
    initSpritesSheet();

    // Reusing the exact same keys and animations as CoinBlock
    animationComponent.addAnimation("Active", {spritesSheet["CoinBlock1"], spritesSheet["CoinBlock2"], spritesSheet["CoinBlock3"]});
    animationComponent.addAnimation("Empty", {spritesSheet["EmptyBlock"]});
}

void MushroomBlock::initSpritesSheet() {
    // Reusing the coordinates from CoinBlock
    spritesSheet["CoinBlock1"] = sf::IntRect({2, 96}, {16, 16});
    spritesSheet["CoinBlock2"] = sf::IntRect({19, 96}, {16, 16});
    spritesSheet["CoinBlock3"] = sf::IntRect({36, 96}, {16, 16});
    spritesSheet["EmptyBlock"] = sf::IntRect({2, 113}, {16, 16});
}

void MushroomBlock::reactToCollision(int collidedSide) {
    if (collidedSide == COLLISION_BOTTOM) {
        if (!isEmpty && !isBouncing) {
            
            // Spawn Mushroom (Temporarily removed for testing)
            if (onSpawnItem) {
                Mushroom* newMushroom = new Mushroom(); 
                newMushroom->setPosition({this->position.x, this->position.y - 16.f}); 
                onSpawnItem(newMushroom);
                // onSpawnItem(nullptr); // Just to trigger the callback for testing
            }

            isEmpty = true;

            // TODO: USER TASK
            // 1. Trigger the bounce by setting `isBouncing` to true.
            // 2. Set the initial `bounceVelocity` (e.g., to a negative value to move up).
            isBouncing = true;
            bounceVelocity = -100.f; // Needs to be negative to move UP in SFML
        }
    }
}

void MushroomBlock::update(float dt) {
    // Lazy initialize originalY when update is first called
    if (originalY < 0.f) {
        originalY = position.y;
    }

    // Play appropriate animation
    if (!isEmpty) {
        animationComponent.play("Active", dt);
    } else {
        animationComponent.play("Empty", dt);
    }

    // TODO: USER TASK
    if(isBouncing){
        bounceVelocity = bounceVelocity + bounceGravity * dt;
        position.y = position.y + bounceVelocity * dt;

        if(position.y > originalY){
            isBouncing = false;
            bounceVelocity = 0;
            position.y = originalY;
        }

        this->setPosition(position);
    }
    
}











// If the block `isBouncing`:
// 1. Update `position.y` based on `bounceVelocity * dt`.
// 2. Apply gravity to `bounceVelocity` by adding `bounceGravity * dt`.
// 3. Check if the block has fallen back to or past `originalY`. 
//    If so, snap it back to `originalY`, set `isBouncing` to false, and reset `bounceVelocity` to 0.
// 4. Finally, update the sprite's position using `entitySprite.setPosition(position);`