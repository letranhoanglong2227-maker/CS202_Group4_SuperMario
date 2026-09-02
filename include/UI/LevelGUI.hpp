#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class UserData;

class LevelGUI {
public:
    LevelGUI();
    void update(float deltaSeconds) noexcept;
    void resetTimer() noexcept;
    void render(sf::RenderTarget& target, const UserData* data,
                int worldId, int levelId, int health,
                bool fireballUnlocked) const;
    [[nodiscard]] int elapsedSeconds() const noexcept;
private:
    sf::Font font;
    float elapsed{};
    bool fontLoaded{};
};
