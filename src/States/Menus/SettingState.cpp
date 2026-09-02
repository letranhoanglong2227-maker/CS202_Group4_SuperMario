#include "States/Menus/SettingState.hpp"

#include "Core/AssetLocator.hpp"
#include "Audio/AudioSystem.hpp"

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
    : State(stack, context) {
    if (const auto path = AssetLocator::find(
            "assets/fonts/Super-Mario-Bros--3.ttf")) {
        m_fontLoaded = m_font.openFromFile(*path);
    }
    if (!m_fontLoaded) {
        std::cerr << "Settings font unavailable.\n";
    }
}

void SettingState::handleEvent(const sf::Event& event) {
    const auto* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) return;
    if (key->code == sf::Keyboard::Key::Escape ||
        key->code == sf::Keyboard::Key::Enter) {
        (void)requestPop();
    } else if (key->code == sf::Keyboard::Key::M && context().audio) {
        context().audio->setMusicEnabled(!context().audio->isMusicEnabled());
    } else if (key->code == sf::Keyboard::Key::S && context().audio) {
        context().audio->setEffectsEnabled(
            !context().audio->areEffectsEnabled());
    } else if (key->code == sf::Keyboard::Key::Left && context().audio) {
        context().audio->setMusicVolume(context().audio->musicVolume() - 5.f);
    } else if (key->code == sf::Keyboard::Key::Right && context().audio) {
        context().audio->setMusicVolume(context().audio->musicVolume() + 5.f);
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
    if (m_fontLoaded) {
        const bool musicEnabled =
            !context().audio || context().audio->isMusicEnabled();
        const bool effectsEnabled =
            !context().audio || context().audio->areEffectsEnabled();
        const int volume = context().audio
            ? static_cast<int>(context().audio->musicVolume()) : 0;
        drawCentered(target, m_font,
                     effectsEnabled ? "SOUND  ON" : "SOUND  OFF", 27,
                     {640.f, 295.f}, effectsEnabled
                         ? sf::Color(100, 225, 125)
                         : sf::Color(255, 135, 100));
        drawCentered(target, m_font,
                     musicEnabled ? "MUSIC  ON" : "MUSIC  OFF", 27,
                     {640.f, 355.f}, musicEnabled
                         ? sf::Color(100, 225, 125)
                         : sf::Color(255, 135, 100));
        drawCentered(target, m_font,
                     "VOLUME  " + std::to_string(volume), 27,
                     {640.f, 415.f}, sf::Color(205, 220, 235));
        drawCentered(target, m_font,
                     "S SOUND  M MUSIC  LEFT RIGHT VOLUME  ENTER BACK", 15,
                     {640.f, 490.f}, sf::Color::White);
    }
}
