#pragma once

#include "Core/GameObject.hpp"
#include <functional>
#include <optional>

class WinFlag : public GameObject {
public:
    using CompletionCallback = std::function<void()>;
    using GameObject::setPosition;

    WinFlag(sf::Vector2f position = {}, CompletionCallback callback = {});
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    void setPosition(const sf::Vector2f& position) override;
    void activate();
    bool isActivated() const noexcept;
    sf::Vector2f getFlagVisualPosition() const noexcept;
private:
    void syncGeometry();

    CompletionCallback callback;
    std::optional<sf::Sprite> poleSprite;
    std::optional<sf::Sprite> sphereSprite;
    bool activated{false};
    bool completed{false};
    float animationTime{0.f};
    int currentFrame{0};
};

