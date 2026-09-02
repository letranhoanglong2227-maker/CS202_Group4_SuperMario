#pragma once

#include "States/Base/State.hpp"

#include <SFML/Graphics/Font.hpp>

#include <string>

class WinMenuState final : public State {
public:
    WinMenuState(StateStack& stack, StateContext context,
                 int worldId, int levelId);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;
private:
    void continueToNextStage();

    sf::Font m_font;
    sf::Font m_readableFont;
    int m_worldId;
    int m_levelId;
    bool m_fontLoaded{};
    bool m_readableFontLoaded{};
    std::string m_error;
};
