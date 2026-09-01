#include "Objects/Blocks/MushroomBlock.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Objects/Items/Mushroom.hpp"
#include <algorithm>
#include <utility>

MushroomBlock::MushroomBlock(SpawnCallback spawnCallback,
                             int mushroomType)
    : isEmpty(false), 
      isBouncing(false), 
      bounceTimer(0.f),
      bounceDuration(0.2f),
      maxBounceHeight(10.f),
      onSpawnItem(std::move(spawnCallback)),
      type(std::clamp(mushroomType, 1, 3))
{
    name = "MushroomBlock";
    exist = true;
    originalY = -1.f; 

    // Initialize textures and add animations directly in the constructor
    initSpritesSheet();

    // Reusing the exact same keys and animations as CoinBlock
    animationComponent.addAnimation("Active", {spritesSheet["CoinBlock1"], spritesSheet["CoinBlock2"], spritesSheet["CoinBlock3"]});
    animationComponent.addAnimation("Empty", {spritesSheet["EmptyBlock"]});
}

void MushroomBlock::initSpritesSheet() {
    spritesSheet["CoinBlock1"] = sf::IntRect({1, 52}, {16, 16});
    spritesSheet["CoinBlock2"] = sf::IntRect({18, 52}, {16, 16});
    spritesSheet["CoinBlock3"] = sf::IntRect({35, 52}, {16, 16});
    spritesSheet["EmptyBlock"] = sf::IntRect({1, 69}, {16, 16});
}

void MushroomBlock::reactToCollision(int collidedSide) {
    if (collidedSide == COLLISION_BOTTOM) {
        if (!isEmpty && !isBouncing) {
            
            if (onSpawnItem) {
                onSpawnItem(std::make_unique<Mushroom>(
                    sf::Vector2f{this->position.x,
                                 this->position.y - MapFormat::TILE_SIZE},
                     true,
                     type));
            }

            isEmpty = true;

            isBouncing = true;
            bounceTimer = 0.f;
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

    if(isBouncing){
        bounceTimer = std::min(
            bounceTimer + std::max(0.f, dt), bounceDuration);
        const float phase = bounceDuration > 0.f
            ? bounceTimer / bounceDuration
            : 1.f;
        position.y = originalY -
            std::sin(phase * 3.14159265f) * maxBounceHeight;
        if (bounceTimer >= bounceDuration) {
            isBouncing = false;
            bounceTimer = 0.f;
            position.y = originalY;
        }
        entitySprite.setPosition(position);
    }
}

bool MushroomBlock::isBumpingUpward() const noexcept {
    return isBouncing && bounceTimer <= bounceDuration * 0.5f;
}
