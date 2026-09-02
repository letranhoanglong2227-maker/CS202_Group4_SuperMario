#include "States/Menus/MenuCharacterSelectionState.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"
#include "States/Menus/MenuWorldState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>

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

void drawTitle(sf::RenderTarget& target, const sf::Font& font,
               std::string_view value) {
    sf::Text title(font, value.data(), 42);
    title.setFillColor(sf::Color(255, 220, 70));
    title.setOutlineColor(sf::Color(20, 25, 35));
    title.setOutlineThickness(3.f);
    const auto bounds = title.getLocalBounds();
    title.setOrigin(bounds.position + bounds.size / 2.f);
    title.setPosition({640.f, 180.f});
    target.draw(title);
}
}

MenuCharacterSelectionState::MenuCharacterSelectionState(
    StateStack& stack, StateContext context)
    : State(stack, context),
      m_menu({
          {"MARIO", {{270.f, 360.f}, {300.f, 90.f}}, true},
          {"LUIGI", {{710.f, 360.f}, {300.f, 90.f}}, true},
      }) {
    if (const auto path = AssetLocator::find(
            "assets/textures/selection_character_background.png"))
        m_backgroundLoaded = m_backgroundTexture.loadFromFile(*path);
    if (const auto path = AssetLocator::find(
            "assets/fonts/Super-Mario-Bros--3.ttf"))
        m_fontLoaded = m_font.openFromFile(*path);
}

void MenuCharacterSelectionState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>();
        key && key->code == sf::Keyboard::Key::Escape) {
        (void)requestPop();
        return;
    }
    if (const auto selected = m_menu.handleEvent(
            event, pointerPosition(event, context().window)))
        activate(*selected);
}

void MenuCharacterSelectionState::update(float) {}

void MenuCharacterSelectionState::render(sf::RenderTarget& target) {
    const auto view = target.getView();
    const auto topLeft = view.getCenter() - view.getSize() / 2.f;
    if (m_backgroundLoaded) {
        sf::Sprite background(m_backgroundTexture);
        const auto size = m_backgroundTexture.getSize();
        background.setPosition(topLeft);
        background.setScale({view.getSize().x / static_cast<float>(size.x),
                             view.getSize().y / static_cast<float>(size.y)});
        target.draw(background);
    } else {
        sf::RectangleShape background(view.getSize());
        background.setPosition(topLeft);
        background.setFillColor(sf::Color(65, 145, 205));
        target.draw(background);
    }
    sf::RectangleShape shade(view.getSize());
    shade.setPosition(topLeft);
    shade.setFillColor(sf::Color(8, 18, 35, 95));
    target.draw(shade);
    if (m_fontLoaded) {
        drawTitle(target, m_font, "CHOOSE YOUR CHARACTER");
        m_menu.render(target, m_font, 30);
    }
}

void MenuCharacterSelectionState::activate(std::size_t selected) {
    if (context().userData)
        context().userData->setCharacter(selected == 1 ? 1 : 0);
    (void)requestReplace(std::make_unique<MenuWorldState>(
        stateStack(), context()));
}
