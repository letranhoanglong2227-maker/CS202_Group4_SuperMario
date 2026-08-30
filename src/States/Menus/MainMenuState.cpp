#include "States/Menus/MainMenuState.hpp"

#include "States/Menus/EnterNameState.hpp"
#include "States/Menus/SettingState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <string_view>

namespace {
constexpr std::array<std::string_view, 4> Options{
    "NEW GAME", "SETTINGS", "LEADERBOARD  P4", "EXIT"};
constexpr float ButtonX = 790.f;
constexpr float ButtonY = 285.f;
constexpr float ButtonWidth = 390.f;
constexpr float ButtonHeight = 64.f;
constexpr float ButtonGap = 18.f;

void drawCenteredText(sf::RenderTarget& target, const sf::Font& font, std::string_view value,
                      unsigned size, sf::Vector2f center, sf::Color color) {
    sf::Text text(font, value.data(), size);
    text.setFillColor(color);
    text.setOutlineColor(sf::Color(20, 25, 35));
    text.setOutlineThickness(2.f);
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.position + bounds.size / 2.f);
    text.setPosition(center);
    target.draw(text);
}
}

MainMenuState::MainMenuState(StateStack& stack, StateContext context)
    : State(stack, context),
      m_backgroundLoaded(m_backgroundTexture.loadFromFile("assets/textures/basic_mainmenu.png")),
      m_fontLoaded(m_font.openFromFile("assets/fonts/Super-Mario-Bros--3.ttf")) {
    if (!m_backgroundLoaded) {
        std::cerr << "Main menu background unavailable; using color fallback.\n";
    }
    if (!m_fontLoaded) {
        std::cerr << "Main menu font unavailable; controls remain keyboard-operable.\n";
    }
}

void MainMenuState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        switch (key->code) {
        case sf::Keyboard::Key::Up:
            m_selected = (m_selected + Options.size() - 1) % Options.size();
            break;
        case sf::Keyboard::Key::Down:
            m_selected = (m_selected + 1) % Options.size();
            break;
        case sf::Keyboard::Key::Enter:
            activateSelection();
            break;
        case sf::Keyboard::Key::Escape:
            (void)requestQuit();
            break;
        default:
            break;
        }
    }

    if (context().window) {
        if (const auto* move = event.getIf<sf::Event::MouseMoved>()) {
            const int hovered = optionAt(context().window->mapPixelToCoords(move->position));
            if (hovered >= 0) {
                m_selected = static_cast<std::size_t>(hovered);
            }
        }
        if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>();
            click && click->button == sf::Mouse::Button::Left) {
            const int clicked = optionAt(context().window->mapPixelToCoords(click->position));
            if (clicked >= 0) {
                m_selected = static_cast<std::size_t>(clicked);
                activateSelection();
            }
        }
    }
}

void MainMenuState::update(float) {}

void MainMenuState::render(sf::RenderTarget& target) {
    const sf::View& view = target.getView();
    const sf::Vector2f topLeft = view.getCenter() - view.getSize() / 2.f;

    if (m_backgroundLoaded) {
        sf::Sprite background(m_backgroundTexture);
        const sf::Vector2u textureSize = m_backgroundTexture.getSize();
        background.setPosition(topLeft);
        background.setScale({view.getSize().x / static_cast<float>(textureSize.x),
                             view.getSize().y / static_cast<float>(textureSize.y)});
        target.draw(background);
    } else {
        sf::RectangleShape fallback(view.getSize());
        fallback.setPosition(topLeft);
        fallback.setFillColor(sf::Color(45, 145, 210));
        target.draw(fallback);
    }

    sf::RectangleShape panel({470.f, 610.f});
    panel.setPosition({745.f, 55.f});
    panel.setFillColor(sf::Color(14, 24, 44, 218));
    panel.setOutlineColor(sf::Color(255, 220, 80));
    panel.setOutlineThickness(3.f);
    target.draw(panel);

    for (std::size_t i = 0; i < Options.size(); ++i) {
        const float y = ButtonY + static_cast<float>(i) * (ButtonHeight + ButtonGap);
        sf::RectangleShape button({ButtonWidth, ButtonHeight});
        button.setPosition({ButtonX, y});
        const bool disabled = i == 2;
        button.setFillColor(disabled ? sf::Color(55, 62, 74, 215)
                                     : (i == m_selected ? sf::Color(190, 52, 46, 238)
                                                        : sf::Color(22, 36, 58, 225)));
        button.setOutlineColor(i == m_selected ? sf::Color(255, 226, 92) : sf::Color(225, 235, 245));
        button.setOutlineThickness(i == m_selected ? 3.f : 1.f);
        target.draw(button);

        if (m_fontLoaded) {
            drawCenteredText(target, m_font, Options[i], 24, {ButtonX + ButtonWidth / 2.f, y + ButtonHeight / 2.f},
                             disabled ? sf::Color(160, 165, 175) : sf::Color::White);
        }
    }

    if (m_fontLoaded) {
        drawCenteredText(target, m_font, "SUPER MARIO", 36, {980.f, 120.f}, sf::Color(255, 218, 62));
        drawCenteredText(target, m_font, "GROUP 4  -  UI PREVIEW", 20, {980.f, 180.f}, sf::Color(238, 245, 255));
        drawCenteredText(target, m_font, "ARROWS / MOUSE TO SELECT   ENTER TO CONFIRM", 13,
                         {980.f, 625.f}, sf::Color(205, 220, 235));
    }
}

void MainMenuState::activateSelection() {
    switch (m_selected) {
    case 0:
        (void)requestPush(std::make_unique<EnterNameState>(stateStack(), context()));
        break;
    case 1:
        (void)requestPush(std::make_unique<SettingState>(stateStack(), context()));
        break;
    case 2:
        break; // P4 owns LeaderboardState; keep the dependency visible and non-clickable.
    case 3:
        (void)requestQuit();
        break;
    default:
        break;
    }
}

int MainMenuState::optionAt(sf::Vector2f position) const {
    for (std::size_t i = 0; i < Options.size(); ++i) {
        const float y = ButtonY + static_cast<float>(i) * (ButtonHeight + ButtonGap);
        if (sf::FloatRect({ButtonX, y}, {ButtonWidth, ButtonHeight}).contains(position)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
