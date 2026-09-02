#pragma once

#include "States/Base/State.hpp"
#include "UI/GUI.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>

class MenuLevelState final : public State {
public:
    MenuLevelState(StateStack& stack, StateContext context, int worldId);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;

private:
    void launch(std::size_t selected);

    int m_worldId;
    sf::Texture m_backgroundTexture;
    sf::Font m_font;
    sf::Font m_titleFont;
    GUI::SelectionMenu m_menu;
    bool m_backgroundLoaded{};
    bool m_fontLoaded{};
    bool m_titleFontLoaded{};
    std::string m_error;
};
