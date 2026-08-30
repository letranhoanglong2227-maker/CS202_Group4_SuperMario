#pragma once

#include "States/Base/State.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cstddef>

class MainMenuState final : public State {
public:
    MainMenuState(StateStack& stack, StateContext context);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;

private:
    void activateSelection();
    [[nodiscard]] int optionAt(sf::Vector2f position) const;

    sf::Texture m_backgroundTexture;
    sf::Font m_font;
    sf::Font m_readableFont;
    bool m_backgroundLoaded{};
    bool m_fontLoaded{};
    bool m_readableFontLoaded{};
    std::size_t m_selected{};
};
