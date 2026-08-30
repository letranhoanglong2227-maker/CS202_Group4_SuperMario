#include "States/Base/GameState.hpp"

#include <algorithm>

namespace {
float clampedAxis(float focus, float viewportSize, float worldStart, float worldSize) {
    if (worldSize <= viewportSize) {
        return worldStart + worldSize / 2.f;
    }

    const float halfViewport = viewportSize / 2.f;
    return std::clamp(focus, worldStart + halfViewport, worldStart + worldSize - halfViewport);
}
}

std::optional<sf::View> GameState::buildClampedCamera(
    sf::Vector2f focus, sf::Vector2f viewportSize, const sf::FloatRect& worldBounds) {
    if (viewportSize.x <= 0.f || viewportSize.y <= 0.f ||
        worldBounds.size.x <= 0.f || worldBounds.size.y <= 0.f) {
        return std::nullopt;
    }

    sf::View view(sf::FloatRect({0.f, 0.f}, viewportSize));
    view.setCenter({clampedAxis(focus.x, viewportSize.x, worldBounds.position.x, worldBounds.size.x),
                    clampedAxis(focus.y, viewportSize.y, worldBounds.position.y, worldBounds.size.y)});
    return view;
}
