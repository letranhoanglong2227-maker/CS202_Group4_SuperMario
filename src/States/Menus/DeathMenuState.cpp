#include "States/Menus/DeathMenuState.hpp"

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

DeathMenuState::DeathMenuState(
    StateStack& stack, StateContext context, RetryCallback retry)
    : State(stack, context),
      m_menu({
          {"RETRY", {{440.f, 350.f}, {400.f, 75.f}},
           !context.userData || !context.userData->checkGameOver()},
          {"MAIN MENU", {{440.f, 450.f}, {400.f, 75.f}}, true},
      }),
      m_retry(std::move(retry)) {
    if (const auto path = AssetLocator::find(
            "assets/fonts/Super-Mario-Bros--3.ttf"))
        m_fontLoaded = m_font.openFromFile(*path);
}

void DeathMenuState::handleEvent(const sf::Event& event) {
    if (const auto selected = m_menu.handleEvent(
            event, pointerPosition(event, context().window)))
        activate(*selected);
}

void DeathMenuState::update(float) {}

void DeathMenuState::render(sf::RenderTarget& target) {
    const auto view = target.getView();
    sf::RectangleShape background(view.getSize());
    background.setPosition(view.getCenter() - view.getSize() / 2.f);
    background.setFillColor(sf::Color(20, 8, 12));
    target.draw(background);
    if (!m_fontLoaded) return;
    const bool gameOver = context().userData &&
                          context().userData->checkGameOver();
    sf::Text title(m_font, gameOver ? "GAME OVER" : "YOU DIED", 52);
    const auto bounds = title.getLocalBounds();
    title.setOrigin(bounds.position + bounds.size / 2.f);
    title.setPosition({640.f, 210.f});
    title.setFillColor(sf::Color(235, 70, 65));
    target.draw(title);
    m_menu.render(target, m_font, 25);
}

void DeathMenuState::activate(std::size_t selected) {
    if (selected == 0) {
        if (m_retry && m_retry()) (void)requestPop();
    } else {
        (void)requestPopToRoot();
    }
}
