#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class AnimationComponent {
private:
    sf::Sprite& sprite;
    sf::Texture& textureSheet;

    struct Animation {
        std::vector<sf::IntRect> frames;
        float switchTime{ 0.1f };
        bool loop{ true };
    };

    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimationKey;
    float timer{ 0.f };
    std::size_t currentFrameIndex{ 0 };
    bool done{ false };

public:
    AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet);
    ~AnimationComponent() = default;

    void addAnimation(const std::string& key, const std::vector<sf::IntRect>& frames, float switchTime = 0.1f, bool loop = true);
    void play(const std::string& key, float dt);
    void setAnimationFrame(const sf::IntRect& rect);
    bool isDone() const;
    void reset(const std::string& key);
};

