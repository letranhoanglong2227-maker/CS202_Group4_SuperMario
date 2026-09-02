#pragma once

#include "Core/GameEventMediator.hpp"
#include "States/Base/State.hpp"
#include "UI/LevelGUI.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
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
    [[nodiscard]] bool restartLevel();

    [[nodiscard]] static std::optional<sf::View> buildClampedCamera(
        sf::Vector2f focus, sf::Vector2f viewportSize, const sf::FloatRect& worldBounds);

private:
    [[nodiscard]] bool requestShoot(sf::Vector2f direction, float speed);
    void bindLevelCallbacks();
    void clearLevelBindings() noexcept;
    void handleRuntimeEvent(const GameEvent& event);
    [[nodiscard]] GameEventId nextEventId() noexcept;

    int m_worldId;
    int m_levelId;
    std::unique_ptr<PlayerManager> m_player;
    std::unique_ptr<LevelManager> m_activeLevel;
    GameEventMediator m_events;
    GameEventMediator::Subscription m_runtimeSubscription;
    LevelGUI m_hud;
    GameEventId m_nextEventId{1};
    float m_facingDirection{1.f};
    bool m_ready{};
    bool m_backgroundLoaded{};
    bool m_endMenuOpen{};
    sf::Texture m_backgroundTexture;
    std::string m_loadError;
};
