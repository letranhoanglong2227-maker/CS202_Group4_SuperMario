#pragma once

#include "States/Base/State.hpp"

#include <SFML/Graphics/Font.hpp>

#include <string>

class EnterNameState final : public State {
public:
    EnterNameState(StateStack& stack, StateContext context);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;

private:
    [[nodiscard]] bool hasValidName() const;

    sf::Font m_font;
    sf::Font m_readableFont;
    bool m_fontLoaded{};
    bool m_readableFontLoaded{};
    bool m_confirmed{};
    std::string m_playerName;
};
