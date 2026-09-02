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
    void continueToCharacterSelection();

    sf::Font m_font;
    bool m_fontLoaded{};
    bool m_confirmed{};
    std::string m_playerName;
};
