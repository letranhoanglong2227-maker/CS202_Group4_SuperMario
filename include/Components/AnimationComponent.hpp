#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>

class AnimationComponent {
private:
    sf::Sprite& sprite;
    sf::Texture& textureSheet;

    std::unordered_map<std::string, std::vector<sf::IntRect>> animations;

    float animationTimer;
    float animationInterval;
    int currentFrame;
    std::string currentAnimationKey;

public:
    // Constructor takes references to the owner's sprite and texture, with a default interval of 0.1f
    AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet, float interval = 0.1f);
    ~AnimationComponent() = default;

    // Disable copy to prevent alias references
    AnimationComponent(const AnimationComponent&) = delete;
    AnimationComponent& operator=(const AnimationComponent&) = delete;

    // Set the interval speed for all animations on this component
    void setInterval(float interval);

    // Register a new animation "tape"
    bool addAnimation(const std::string& key, const std::vector<sf::IntRect>& frames);

    // Play/Update an animation
    bool play(const std::string& key, float dt);
};
