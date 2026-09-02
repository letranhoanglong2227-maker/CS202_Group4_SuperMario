#pragma once

#include "States/Base/State.hpp"
#include "UI/GUI.hpp"

#include <SFML/Graphics/Font.hpp>

#include <functional>

class DeathMenuState final : public State {
public:
    using RetryCallback = std::function<bool()>;
    DeathMenuState(StateStack& stack, StateContext context,
                   RetryCallback retry);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;
private:
    void activate(std::size_t selected);
    sf::Font m_font;
    GUI::SelectionMenu m_menu;
    RetryCallback m_retry;
    bool m_fontLoaded{};
};
