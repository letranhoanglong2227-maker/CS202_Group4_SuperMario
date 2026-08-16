#include "Objects/Blocks/Brick.hpp"
#include "Objects/Blocks/BrickFragment.hpp"

Brick::Brick(std::function<void(GameObject*)> spawnCallback)
    : isBroken(false), onSpawnItem(spawnCallback) 
{
    name = "Brick";
    exist = true; // Make sure it exists initially
    
    // Initialize textures and add animations directly in the constructor
    initSpritesSheet();

    // Set up the Idle animation (1 frame)
    animationComponent.addAnimation("Idle", {spritesSheet["IdleFrame"]});
}

void Brick::initSpritesSheet() {
    // TODO: USER TASK - Update this coordinate for the brick Idle frame
    spritesSheet["IdleFrame"] = sf::IntRect({19, 113}, {16, 16});
}

void Brick::reactToCollision(int collidedSide) {
    // We assume this method is ONLY called when Super Mario hits the block from the bottom.
    // The external collision manager should handle the logic of checking Mario's state.
    if (collidedSide == COLLISION_BOTTOM && !isBroken) {
        isBroken = true;
        
        // Stop rendering the brick by setting alpha to 0 AND exist to false
        sf::Color transparent = entitySprite.getColor();
        transparent.a = 0;
        entitySprite.setColor(transparent);
        exist = false; // Flag for deletion

        // Spawn 4 BrickFragments
        if (onSpawnItem) {
            // These initial velocities create an arc effect. 
            // TODO: Physics Tuning Task! Tweak these X and Y values to make the explosion look perfect!
            sf::Vector2f velTopLeft(-150.f, -400.f);
            sf::Vector2f velTopRight(150.f, -400.f);
            sf::Vector2f velBotLeft(-200.f, -200.f);
            sf::Vector2f velBotRight(200.f, -200.f);

            // Create and spawn them
            onSpawnItem(new BrickFragment(position, velTopLeft));
            onSpawnItem(new BrickFragment(position, velTopRight));
            onSpawnItem(new BrickFragment(position, velBotLeft));
            onSpawnItem(new BrickFragment(position, velBotRight));
        }
    }
}

void Brick::update(float dt) {
    if (isBroken || !exist) return;

    // Play Idle animation
    animationComponent.play("Idle", dt);
}
