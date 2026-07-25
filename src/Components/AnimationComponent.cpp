#include "Components/AnimationComponent.hpp"

AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet)
    : sprite(sprite), textureSheet(textureSheet)
{
    this->sprite.setTexture(this->textureSheet);
}

void AnimationComponent::addAnimation(const std::string& key, const std::vector<sf::IntRect>& frames, float switchTime, bool loop)
{
    Animation anim;
    anim.frames = frames;
    anim.switchTime = switchTime;
    anim.loop = loop;
    this->animations[key] = anim;
}

void AnimationComponent::play(const std::string& key, float dt)
{
    auto it = this->animations.find(key);
    if (it == this->animations.end()) return;

    Animation& anim = it->second;
    if (anim.frames.empty()) return;

    if (this->currentAnimationKey != key) {
        this->currentAnimationKey = key;
        this->currentFrameIndex = 0;
        this->timer = 0.f;
        this->done = false;
        this->setAnimationFrame(anim.frames[0]);
    }

    this->timer += dt;
    if (this->timer >= anim.switchTime) {
        this->timer -= anim.switchTime;
        if (this->currentFrameIndex + 1 < anim.frames.size()) {
            this->currentFrameIndex++;
        } else {
            if (anim.loop) {
                this->currentFrameIndex = 0;
            } else {
                this->done = true;
            }
        }
        this->setAnimationFrame(anim.frames[this->currentFrameIndex]);
    }
}

void AnimationComponent::setAnimationFrame(const sf::IntRect& rect)
{
    this->sprite.setTextureRect(rect);
}

bool AnimationComponent::isDone() const
{
    return this->done;
}

void AnimationComponent::reset(const std::string& key)
{
    auto it = this->animations.find(key);
    if (it != this->animations.end()) {
        this->currentAnimationKey = key;
        this->currentFrameIndex = 0;
        this->timer = 0.f;
        this->done = false;
        if (!it->second.frames.empty()) {
            this->setAnimationFrame(it->second.frames[0]);
        }
    }
}
