#pragma once

#include "Core/GameObject.hpp"
#include <functional>

class WinFlag : public GameObject {
public:
    using CompletionCallback = std::function<void()>;
    WinFlag(sf::Vector2f position = {}, CompletionCallback callback = {});
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    void activate();
    bool isActivated() const noexcept;
private:
    CompletionCallback callback;
    sf::RectangleShape pole;
    sf::ConvexShape flag;
    bool activated{false};
};

