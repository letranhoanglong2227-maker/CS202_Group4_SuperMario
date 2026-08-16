#include "Objects/Blocks/BrickFragment.hpp"

BrickFragment::BrickFragment(sf::Vector2f startPos, sf::Vector2f initialVelocity)
    : velocity(initialVelocity), gravity(1500.f) 
{
    name = "BrickFragment";
    position = startPos;
    exist = true; // Ensure fragment updates and renders
    
    initSpritesSheet();
    
    // Set up the rotating animation (2 frames)
    animationComponent.addAnimation("Rotate", {spritesSheet["Fragment1"], spritesSheet["Fragment2"]});
    
    // Small optimization: Fragments are usually smaller than full blocks
    // This is optional and depends on your exact sprite sheet
    setSize({8.f, 8.f});
}

void BrickFragment::initSpritesSheet() {
    // TODO: USER TASK - Update these coordinates for the 2 rotation frames
    spritesSheet["Fragment1"] = sf::IntRect({53, 113}, {8, 8});
    spritesSheet["Fragment2"] = sf::IntRect({71, 113}, {8, 8});
}

void BrickFragment::reactToCollision(int collidedSide) {
    // Fragments are purely visual and don't react to collisions
}

void BrickFragment::update(float dt) {
    if (!exist) return; // If already marked for deletion, do nothing

    // Play rotation animation
    animationComponent.play("Rotate", dt);

    // Apply physics
    velocity.y += gravity * dt;
    position += velocity * dt;

    // Update the underlying sprite position
    entitySprite.setPosition(position);

    // Deallocate if it falls too far below the screen
    if (position.y > 1080.f) {
        exist = false; // Safely mark for cleanup
    }
}
