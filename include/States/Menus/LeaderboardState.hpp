#pragma once

#include "States/Base/State.hpp"
#include "Core/UserData.hpp"
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <vector>

class LeaderboardState final : public State {
public:
    LeaderboardState(StateStack& stack, StateContext context);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;
private:
    sf::Font font;
    bool fontLoaded{};
    std::vector<LeaderboardEntry> entries;
    std::string loadError;
};
