#include "UI/LevelGUI.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <algorithm>
#include <array>
#include <string>

LevelGUI::LevelGUI() {
    if (const auto path = AssetLocator::find("assets/fonts/pixel-nes.otf"))
        fontLoaded = font.openFromFile(*path);
}

void LevelGUI::update(float deltaSeconds) noexcept {
    elapsed += std::max(0.f, deltaSeconds);
}

void LevelGUI::resetTimer() noexcept {
    elapsed = 0.f;
}

void LevelGUI::render(sf::RenderTarget& target, const UserData* data,
                      int worldId, int levelId, int health,
                      bool fireballUnlocked) const {
    const sf::View view = target.getView();
    const sf::Vector2f topLeft = view.getCenter() - view.getSize() / 2.f;
    sf::RectangleShape bar(
        {view.getSize().x, fireballUnlocked ? 84.f : 62.f});
    bar.setPosition(topLeft);
    bar.setFillColor(sf::Color(5, 10, 20, 175));
    target.draw(bar);
    if (!fontLoaded) return;

    const std::string name = data ? data->getPlayerName() : "PLAYER";
    const int lives = data ? data->getLives() : 3;
    const int coins = data ? data->getNumberOfCoins() : 0;
    const int score = data ? data->getScore() : 0;
    const std::array labels{
        name + "  HP " + std::to_string(std::max(0, health)) +
            "  LIFE " + std::to_string(lives),
        "COIN " + std::to_string(coins),
        "WORLD " + std::to_string(worldId) + "-" + std::to_string(levelId),
        "TIME " + std::to_string(elapsedSeconds()),
        "SCORE " + std::to_string(score),
    };
    const float section = view.getSize().x /
                          static_cast<float>(labels.size());
    for (std::size_t index = 0; index < labels.size(); ++index) {
        sf::Text text(font, labels[index], 18);
        text.setFillColor(sf::Color::White);
        const auto bounds = text.getLocalBounds();
        text.setOrigin(bounds.position + bounds.size / 2.f);
        text.setPosition({topLeft.x + section * (index + 0.5f),
                          topLeft.y + 30.f});
        target.draw(text);
    }
    if (fireballUnlocked) {
        sf::Text powerHint(
            font, "FIREBALL READY - LEFT CLICK OR K TO SHOOT", 14);
        powerHint.setFillColor(sf::Color(255, 220, 70));
        const auto bounds = powerHint.getLocalBounds();
        powerHint.setOrigin(bounds.position + bounds.size / 2.f);
        powerHint.setPosition(
            {topLeft.x + view.getSize().x / 2.f, topLeft.y + 68.f});
        target.draw(powerHint);
    }
}

int LevelGUI::elapsedSeconds() const noexcept {
    return static_cast<int>(elapsed);
}
