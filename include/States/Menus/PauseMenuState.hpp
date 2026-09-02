#pragma once

#include "States/Base/State.hpp"
#include "UI/GUI.hpp"

#include <SFML/Graphics/Font.hpp>

#include <functional>
#include <string>

class PauseMenuState final : public State {
public:
    using RestartCallback = std::function<bool()>;
    PauseMenuState(StateStack& stack, StateContext context,
                   RestartCallback restart);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;
private:
    void activate(std::size_t selected);
    sf::Font m_font;
    GUI::SelectionMenu m_menu;
    RestartCallback m_restart;
    bool m_fontLoaded{};
    std::string m_message;
};
