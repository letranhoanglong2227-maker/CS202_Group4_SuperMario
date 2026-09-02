#pragma once

#include "States/Base/State.hpp"
#include "UI/GUI.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

class MenuWorldState final : public State {
public:
    MenuWorldState(StateStack& stack, StateContext context);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;

private:
    void activate(std::size_t selected);

    sf::Texture m_backgroundTexture;
    sf::Font m_font;
    sf::Font m_titleFont;
    GUI::SelectionMenu m_menu;
    bool m_backgroundLoaded{};
    bool m_fontLoaded{};
    bool m_titleFontLoaded{};
};
