#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

#include <optional>

class GameState {
public:
    [[nodiscard]] static std::optional<sf::View> buildClampedCamera(
        sf::Vector2f focus, sf::Vector2f viewportSize, const sf::FloatRect& worldBounds);
};
