#include "States/Menus/SettingState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <iostream>
#include <string_view>

namespace {
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

SettingState::SettingState(StateStack& stack, StateContext context)
    : State(stack, context),
      m_fontLoaded(m_font.openFromFile("assets/fonts/Super-Mario-Bros--3.ttf")),
      m_readableFontLoaded(m_readableFont.openFromFile("assets/fonts/American Captain.ttf")) {
    if (!m_fontLoaded) {
        std::cerr << "Settings font unavailable.\n";
    }
    if (!m_readableFontLoaded) {
        std::cerr << "Settings readable font unavailable.\n";
    }
}

void SettingState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>();
        key && (key->code == sf::Keyboard::Key::Escape || key->code == sf::Keyboard::Key::Enter)) {
        (void)requestPop();
    }
}

void SettingState::update(float) {}

void SettingState::render(sf::RenderTarget& target) {
    const sf::View& view = target.getView();
    sf::RectangleShape background(view.getSize());
    background.setPosition(view.getCenter() - view.getSize() / 2.f);
    background.setFillColor(sf::Color(22, 30, 48));
    target.draw(background);

    sf::RectangleShape card({780.f, 390.f});
    card.setPosition({250.f, 150.f});
    card.setFillColor(sf::Color(12, 20, 38, 235));
    card.setOutlineColor(sf::Color(255, 218, 62));
    card.setOutlineThickness(3.f);
    target.draw(card);

    if (m_fontLoaded) {
        drawCentered(target, m_font, "SETTINGS", 38, {640.f, 220.f}, sf::Color(255, 218, 62));
    }
    if (m_readableFontLoaded) {
        drawCentered(target, m_readableFont, "AUDIO CONTROLS ARE WAITING FOR THE P4 AUDIOSYSTEM CONTRACT", 23,
                     {640.f, 330.f}, sf::Color(255, 185, 85));
        drawCentered(target, m_readableFont, "NO DUPLICATE VOLUME STORE WAS ADDED", 22,
                     {640.f, 385.f}, sf::Color(205, 220, 235));
        drawCentered(target, m_readableFont, "ENTER OR ESC TO GO BACK", 25,
                     {640.f, 475.f}, sf::Color::White);
    }
}
