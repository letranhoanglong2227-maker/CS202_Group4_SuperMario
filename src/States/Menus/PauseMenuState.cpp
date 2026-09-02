#include "States/Menus/PauseMenuState.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <utility>

namespace {
std::optional<sf::Vector2f> pointerPosition(
    const sf::Event& event, sf::RenderWindow* window) {
    if (!window) return std::nullopt;
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
        return window->mapPixelToCoords(moved->position);
    if (const auto* clicked = event.getIf<sf::Event::MouseButtonPressed>())
        return window->mapPixelToCoords(clicked->position);
    return std::nullopt;
}
}

PauseMenuState::PauseMenuState(
    StateStack& stack, StateContext context, RestartCallback restart)
    : State(stack, context),
      m_menu({
          {"CONTINUE", {{440.f, 260.f}, {400.f, 64.f}}, true},
          {"RESTART LEVEL", {{440.f, 340.f}, {400.f, 64.f}}, true},
          {"SAVE GAME", {{440.f, 420.f}, {400.f, 64.f}}, true},
          {"MAIN MENU", {{440.f, 500.f}, {400.f, 64.f}}, true},
      }),
      m_restart(std::move(restart)) {
    if (const auto path = AssetLocator::find(
            "assets/fonts/Super-Mario-Bros--3.ttf"))
        m_fontLoaded = m_font.openFromFile(*path);
}

void PauseMenuState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>();
        key && key->code == sf::Keyboard::Key::Escape) {
        (void)requestPop();
        return;
    }
    if (const auto selected = m_menu.handleEvent(
            event, pointerPosition(event, context().window)))
        activate(*selected);
}

void PauseMenuState::update(float) {}

void PauseMenuState::render(sf::RenderTarget& target) {
    const auto view = target.getView();
    sf::RectangleShape background(view.getSize());
    background.setPosition(view.getCenter() - view.getSize() / 2.f);
    background.setFillColor(sf::Color(8, 12, 24));
    target.draw(background);
    if (!m_fontLoaded) return;
    sf::Text title(m_font, "PAUSED", 48);
    const auto bounds = title.getLocalBounds();
    title.setOrigin(bounds.position + bounds.size / 2.f);
    title.setPosition({640.f, 190.f});
    title.setFillColor(sf::Color(255, 220, 70));
    target.draw(title);
    m_menu.render(target, m_font, 24);
    if (!m_message.empty()) {
        sf::Text message(m_font, m_message, 18);
        const auto messageBounds = message.getLocalBounds();
        message.setOrigin(messageBounds.position + messageBounds.size / 2.f);
        message.setPosition({640.f, 610.f});
        message.setFillColor(sf::Color(255, 220, 70));
        target.draw(message);
    }
}

void PauseMenuState::activate(std::size_t selected) {
    if (selected == 0) (void)requestPop();
    else if (selected == 1) {
        if (m_restart && m_restart()) (void)requestPop();
    } else if (selected == 2) {
        std::string error;
        const auto directory = AssetLocator::executableDirectory();
        const bool saved = context().userData &&
            context().userData->saveTo(directory / "savegame.txt", &error);
        const bool ranked = saved && context().userData->updateLeaderboard(
            directory / "leaderboard.txt", &error);
        m_message = ranked ? "GAME AND SCORE SAVED" : error;
    } else if (selected == 3) {
        (void)requestPopToRoot();
    }
}
