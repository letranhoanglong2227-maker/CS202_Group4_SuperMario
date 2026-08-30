#pragma once

#include "Core/GameEventMediator.hpp"
#include "States/Base/State.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

#include <memory>
#include <optional>
#include <string>

class LevelManager;
class PlayerManager;

class GameState final : public State {
public:
    GameState(StateStack& stack, StateContext context, int worldId, int levelId,
              std::unique_ptr<PlayerManager> player,
              std::unique_ptr<LevelManager> activeLevel);
    ~GameState() override;

    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void render(sf::RenderTarget& target) override;

    [[nodiscard]] bool isReady() const noexcept;
    [[nodiscard]] const std::string& loadError() const noexcept;
    [[nodiscard]] PlayerManager& activePlayer() noexcept;
    [[nodiscard]] const PlayerManager& activePlayer() const noexcept;
    [[nodiscard]] LevelManager& activeLevel() noexcept;
    [[nodiscard]] const LevelManager& activeLevel() const noexcept;
    [[nodiscard]] GameEventMediator& events() noexcept;
    [[nodiscard]] const GameEventMediator& events() const noexcept;
    [[nodiscard]] bool requestShoot(float direction);

    [[nodiscard]] static std::optional<sf::View> buildClampedCamera(
        sf::Vector2f focus, sf::Vector2f viewportSize, const sf::FloatRect& worldBounds);

private:
    void bindLevelCallbacks();
    void clearLevelBindings() noexcept;
    [[nodiscard]] GameEventId nextEventId() noexcept;

    int m_worldId;
    int m_levelId;
    std::unique_ptr<PlayerManager> m_player;
    std::unique_ptr<LevelManager> m_activeLevel;
    GameEventMediator m_events;
    GameEventId m_nextEventId{1};
    float m_facingDirection{1.f};
    bool m_ready{};
    std::string m_loadError;
};
