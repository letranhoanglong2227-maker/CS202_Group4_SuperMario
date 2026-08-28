#pragma once

#include "Core/GameObject.hpp"
#include <functional>
#include <optional>

class Rocket : public GameObject {
public:
    using TargetResolver = std::function<std::optional<sf::Vector2f>()>;

    Rocket(sf::Vector2f position = {}, TargetResolver targetResolver = {},
           float speed = 140.f, float lifetime = 8.f);
    Rocket(sf::Vector2f position, sf::Vector2f velocity,
           float lifetime = 8.f);
    void setTargetResolver(TargetResolver targetResolver);
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
    bool isActive() const noexcept;
    void deactivate() noexcept;
    bool deactivateOnWorldCollision(const sf::FloatRect& obstacle) noexcept;
    bool cullOutside(const sf::FloatRect& worldBounds,
                     float margin = 0.f) noexcept;
private:
    TargetResolver targetResolver;
    float speed;
    float lifetime;
    sf::Vector2f velocity{};
    sf::RectangleShape shape;
    bool active{true};
};

