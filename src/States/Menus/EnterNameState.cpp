#include "States/Menus/EnterNameState.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"
#include "States/Menus/MenuCharacterSelectionState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <iostream>
#include <string_view>

namespace {
void drawText(sf::RenderTarget& target, const sf::Font& font, std::string_view value,
              unsigned size, sf::Vector2f position, sf::Color color) {
    sf::Text text(font, value.data(), size);
    text.setPosition(position);
    text.setFillColor(color);
    target.draw(text);
}

void drawCentered(sf::RenderTarget& target, const sf::Font& font, std::string_view value,
                  unsigned size, sf::Vector2f center, sf::Color color) {
    sf::Text text(font, value.data(), size);
    text.setFillColor(color);
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.position + bounds.size / 2.f);
    text.setPosition(center);
    target.draw(text);
}
}

EnterNameState::EnterNameState(StateStack& stack, StateContext context)
    : State(stack, context) {
    if (const auto path = AssetLocator::find(
            "assets/fonts/American Captain.ttf")) {
        m_fontLoaded = m_font.openFromFile(*path);
    }
    if (!m_fontLoaded) {
        std::cerr << "Name-entry font unavailable.\n";
    }
}

void EnterNameState::handleEvent(const sf::Event& event) {
    if (const auto* text = event.getIf<sf::Event::TextEntered>();
        text && !m_confirmed && text->unicode >= 32 && text->unicode <= 126 && m_playerName.size() < 16) {
        m_playerName.push_back(static_cast<char>(text->unicode));
    }

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Escape) {
            (void)requestPop();
        } else if (key->code == sf::Keyboard::Key::Backspace && !m_confirmed && !m_playerName.empty()) {
            m_playerName.pop_back();
        } else if (key->code == sf::Keyboard::Key::Enter && hasValidName()) {
            m_confirmed = true;
            continueToCharacterSelection();
        }
    }
}

void EnterNameState::update(float) {}

void EnterNameState::render(sf::RenderTarget& target) {
    const sf::View& view = target.getView();
    sf::RectangleShape background(view.getSize());
    background.setPosition(view.getCenter() - view.getSize() / 2.f);
    background.setFillColor(sf::Color(20, 33, 66));
    target.draw(background);

    sf::RectangleShape card({760.f, 390.f});
    card.setPosition({260.f, 155.f});
    card.setFillColor(sf::Color(12, 20, 38, 235));
    card.setOutlineColor(sf::Color(255, 218, 62));
    card.setOutlineThickness(3.f);
    target.draw(card);

    sf::RectangleShape input({600.f, 72.f});
    input.setPosition({340.f, 300.f});
    input.setFillColor(sf::Color(235, 240, 247));
    input.setOutlineColor(hasValidName() ? sf::Color(65, 190, 100) : sf::Color(210, 70, 65));
    input.setOutlineThickness(3.f);
    target.draw(input);

    if (m_fontLoaded) {
        drawText(target, m_font, "PLAYER NAME", 34, {455.f, 205.f}, sf::Color(255, 218, 62));
        const std::string visibleName = m_playerName.empty() ? "TYPE HERE" : m_playerName;
        drawText(target, m_font, visibleName, 34, {365.f, 310.f},
                 m_playerName.empty() ? sf::Color(125, 135, 150) : sf::Color(24, 32, 48));
        drawCentered(target, m_font,
                     "MAX 16 CHARACTERS  -  ENTER TO CONFIRM  -  ESC TO BACK", 21,
                     {640.f, 410.f}, sf::Color(220, 230, 242));
        if (m_confirmed) {
            drawCentered(target, m_font,
                         "CONTINUING TO CHARACTER SELECT", 23,
                         {640.f, 465.f}, sf::Color(100, 225, 125));
        }
    }
}

bool EnterNameState::hasValidName() const {
    return std::any_of(m_playerName.begin(), m_playerName.end(), [](char value) { return value != ' '; });
}

void EnterNameState::continueToCharacterSelection() {
    if (context().userData) context().userData->setPlayerName(m_playerName);
    (void)requestReplace(std::make_unique<MenuCharacterSelectionState>(
        stateStack(), context()));
}
