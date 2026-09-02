#include "States/Menus/MenuWorldState.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"
#include "States/Menus/MenuLevelState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <vector>

namespace {
std::vector<GUI::MenuItem> worldItems(const UserData* data) {
    (void)data;
    return {
        {"WORLD 1", {{160.f, 360.f}, {280.f, 90.f}}, true},
        {"WORLD 2", {{500.f, 360.f}, {280.f, 90.f}}, true},
        {"WORLD 3", {{840.f, 360.f}, {280.f, 90.f}}, true},
    };
}

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

MenuWorldState::MenuWorldState(StateStack& stack, StateContext context)
    : State(stack, context),
      m_menu(worldItems(context.userData)) {
    if (const auto path = AssetLocator::find(
            "assets/textures/selectionBackground2.png"))
        m_backgroundLoaded = m_backgroundTexture.loadFromFile(*path);
    if (const auto path = AssetLocator::find(
            "assets/fonts/Super-Mario-Bros--3.ttf"))
        m_fontLoaded = m_font.openFromFile(*path);
    if (const auto path = AssetLocator::find(
            "assets/fonts/American Captain.ttf"))
        m_titleFontLoaded = m_titleFont.openFromFile(*path);
}

void MenuWorldState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>();
        key && key->code == sf::Keyboard::Key::Escape) {
        (void)requestPop();
        return;
    }
    if (const auto selected = m_menu.handleEvent(
            event, pointerPosition(event, context().window)))
        activate(*selected);
}

void MenuWorldState::update(float) {}

void MenuWorldState::render(sf::RenderTarget& target) {
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
        background.setFillColor(sf::Color(40, 105, 160));
        target.draw(background);
    }
    if (m_fontLoaded) {
        const sf::Font& titleFont = m_titleFontLoaded ? m_titleFont : m_font;
        sf::Text title(titleFont, "CHOOSE A WORLD", 42);
        const auto bounds = title.getLocalBounds();
        title.setOrigin(bounds.position + bounds.size / 2.f);
        title.setPosition({640.f, 190.f});
        title.setFillColor(sf::Color(255, 220, 70));
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3.f);
        target.draw(title);
        m_menu.render(target, m_font, 27);
        sf::Text hint(
            titleFont,
            "ALL MAPS OPEN - EVERY SCORE COUNTS", 22);
        const auto hintBounds = hint.getLocalBounds();
        hint.setOrigin(hintBounds.position + hintBounds.size / 2.f);
        hint.setPosition({640.f, 520.f});
        hint.setFillColor(sf::Color::White);
        hint.setOutlineColor(sf::Color::Black);
        hint.setOutlineThickness(2.f);
        target.draw(hint);
    }
}

void MenuWorldState::activate(std::size_t selected) {
    (void)requestReplace(std::make_unique<MenuLevelState>(
        stateStack(), context(), static_cast<int>(selected) + 1));
}
