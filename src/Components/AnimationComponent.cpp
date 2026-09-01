#include "Components/AnimationComponent.hpp"
#include <iostream>

AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet, float interval)
    : sprite(sprite), 
      textureSheet(textureSheet), 
      animationTimer(0.f), 
      animationInterval(interval <= 0.f ? 0.001f : interval),
      currentFrame(0), 
      currentAnimationKey("Null") 
{
    this->sprite.setTexture(this->textureSheet);
}

void AnimationComponent::setInterval(float interval) {
    this->animationInterval = interval <= 0.f ? 0.001f : interval;
}

bool AnimationComponent::addAnimation(
    const std::string& key, const std::vector<sf::IntRect>& frames) {
    if (frames.empty() || animations.contains(key)) {
        return false;
    }
    animations.emplace(key, frames);
    if (currentAnimationKey == "Null") {
        currentAnimationKey = key;
        currentFrame = 0;
        sprite.setTextureRect(frames.front());
    }
    return true;
}

bool AnimationComponent::play(const std::string& key, float dt) {
    // Check if the requested animation exists
    auto it = animations.find(key);
    if (it == animations.end()) {
        return false;
    }

    // If switching to a new animation, reset the frame and timer
    if (this->currentAnimationKey != key) {
        this->currentAnimationKey = key;
        this->currentFrame = 0;
        this->animationTimer = 0.f;

        // Apply the first frame immediately upon switching
        if (!it->second.empty()) {
            this->sprite.setTextureRect(it->second[this->currentFrame]);
        }
    }

    // Update the animation timer
    this->animationTimer += dt;

    // Transition to the next frame if the timer exceeds the interval
    while (this->animationTimer >= this->animationInterval) {
        this->animationTimer -= this->animationInterval;

        const std::vector<sf::IntRect>& frames = it->second;
        
        if (!frames.empty()) {
            // Move to next frame and loop back to 0 if we reach the end
            this->currentFrame = (this->currentFrame + 1) % frames.size();
            
            // Apply the new frame to the sprite
            this->sprite.setTextureRect(frames[this->currentFrame]);
        }
    }

    return true;
}
