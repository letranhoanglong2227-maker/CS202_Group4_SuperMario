#include "States/Menus/LeaderboardState.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <string>

namespace {
void centeredText(sf::RenderTarget& target, const sf::Font& font,
                  const std::string& value, unsigned size, float y,
                  sf::Color color = sf::Color::White) {
    sf::Text text(font, value, size);
    text.setFillColor(color);
    const auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.position + bounds.size / 2.f);
    text.setPosition({640.f, y});
    target.draw(text);
}

void columnText(sf::RenderTarget& target, const sf::Font& font,
                const std::string& value, unsigned size, sf::Vector2f position,
                float horizontalAnchor,
                sf::Color color = sf::Color::White) {
    sf::Text text(font, value, size);
    text.setFillColor(color);
    const auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x * horizontalAnchor,
                    bounds.position.y + bounds.size.y / 2.f});
    text.setPosition(position);
    target.draw(text);
}
}

LeaderboardState::LeaderboardState(StateStack& stack, StateContext context)
    : State(stack, context) {
    if (const auto path = AssetLocator::find(
            "assets/fonts/American Captain.ttf"))
        fontLoaded = font.openFromFile(*path);
    entries = UserData::loadLeaderboard(
        AssetLocator::executableDirectory() / "leaderboard.txt", &loadError);
}

void LeaderboardState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>();
        key && (key->code == sf::Keyboard::Key::Escape ||
                key->code == sf::Keyboard::Key::Enter ||
                key->code == sf::Keyboard::Key::Backspace))
        (void)requestPop();
    else if (event.is<sf::Event::MouseButtonPressed>())
        (void)requestPop();
}

void LeaderboardState::update(float) {}

void LeaderboardState::render(sf::RenderTarget& target) {
    const sf::View view = target.getView();
    sf::RectangleShape background(view.getSize());
    background.setPosition(view.getCenter() - view.getSize() / 2.f);
    background.setFillColor(sf::Color(35, 42, 105));
    target.draw(background);
    if (!fontLoaded) return;

    centeredText(target, font, "LEADERBOARD", 58, 90.f,
                 sf::Color(255, 218, 70));
    centeredText(target, font, "PLAY ANY MAP - EVERY SCORE COUNTS",
                  22, 128.f, sf::Color(220, 230, 245));
    sf::RectangleShape panel({850.f, 430.f});
    panel.setPosition({215.f, 145.f});
    panel.setFillColor(sf::Color(12, 19, 48, 220));
    panel.setOutlineColor(sf::Color(255, 218, 70));
    panel.setOutlineThickness(3.f);
    target.draw(panel);

    if (!loadError.empty()) {
        centeredText(target, font, loadError, 28, 345.f,
                     sf::Color(255, 120, 100));
    } else if (entries.empty()) {
        centeredText(target, font, "NO SAVED SCORES YET", 32, 345.f);
    } else {
        const sf::Color headingColor(220, 230, 245);
        columnText(target, font, "RANK", 25, {285.f, 185.f}, .5f, headingColor);
        columnText(target, font, "PLAYER", 25, {380.f, 185.f}, 0.f, headingColor);
        columnText(target, font, "SCORE", 25, {820.f, 185.f}, 1.f, headingColor);
        columnText(target, font, "STAGE", 25, {970.f, 185.f}, .5f, headingColor);
        for (std::size_t i = 0; i < entries.size(); ++i) {
            const auto& entry = entries[i];
            const float y = 225.f + static_cast<float>(i) * 32.f;
            columnText(target, font, std::to_string(i + 1), 24,
                       {285.f, y}, .5f);
            columnText(target, font, entry.playerName.substr(0, 16), 24,
                       {380.f, y}, 0.f);
            columnText(target, font, std::to_string(entry.score), 24,
                       {820.f, y}, 1.f);
            columnText(target, font,
                       "W" + std::to_string(entry.world) + "-" +
                           std::to_string(entry.level),
                       24, {970.f, y}, .5f);
        }
    }
    centeredText(target, font, "ENTER / ESC / CLICK TO GO BACK", 24, 630.f,
                 sf::Color(220, 230, 245));
}
