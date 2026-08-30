#include "States/Menus/EnterNameState.hpp"

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
    : State(stack, context),
      m_fontLoaded(m_font.openFromFile("assets/fonts/Super-Mario-Bros--3.ttf")),
      m_readableFontLoaded(m_readableFont.openFromFile("assets/fonts/American Captain.ttf")) {
    if (!m_fontLoaded) {
        std::cerr << "Name-entry font unavailable.\n";
    }
    if (!m_readableFontLoaded) {
        std::cerr << "Name-entry readable font unavailable.\n";
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
        const sf::Font& inputFont = m_readableFontLoaded ? m_readableFont : m_font;
        drawText(target, inputFont, visibleName, 34, {365.f, 310.f},
                 m_playerName.empty() ? sf::Color(125, 135, 150) : sf::Color(24, 32, 48));
        if (m_readableFontLoaded) {
            drawCentered(target, m_readableFont,
                         "MAX 16 CHARACTERS  -  ENTER TO CONFIRM  -  ESC TO BACK", 21,
                         {640.f, 410.f}, sf::Color(220, 230, 242));
        }
        if (m_confirmed) {
            const sf::Font& statusFont = m_readableFontLoaded ? m_readableFont : m_font;
            drawCentered(target, statusFont, "NAME ACCEPTED FOR UI PREVIEW", 23,
                         {640.f, 465.f}, sf::Color(100, 225, 125));
            drawCentered(target, statusFont, "CHARACTER SELECTION WAITS FOR P3 FACTORY AND P4 GUI", 19,
                         {640.f, 505.f}, sf::Color(255, 185, 85));
        }
    }
}

bool EnterNameState::hasValidName() const {
    return std::any_of(m_playerName.begin(), m_playerName.end(), [](char value) { return value != ' '; });
}
