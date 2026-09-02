#pragma once

#include "States/Base/State.hpp"

#include <SFML/Graphics/Font.hpp>

class SettingState final : public State {
public:
    SettingState(StateStack& stack, StateContext context);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;

private:
    sf::Font m_font;
    bool m_fontLoaded{};
};
