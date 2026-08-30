#include "States/Base/GameState.hpp"

#include "Entities/Players/PlayerManager.hpp"
#include "Levels/Managers/LevelManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
float clampedAxis(float focus, float viewportSize, float worldStart, float worldSize) {
    if (worldSize <= viewportSize) {
        return worldStart + worldSize / 2.f;
    }

    const float halfViewport = viewportSize / 2.f;
    return std::clamp(focus, worldStart + halfViewport, worldStart + worldSize - halfViewport);
}

bool isValidStage(int worldId, int levelId) {
    return worldId >= 1 && worldId <= 3 && levelId >= 1 && levelId <= 3;
}
}

GameState::GameState(StateStack& stack, StateContext context, int worldId, int levelId,
                     std::unique_ptr<PlayerManager> player,
                     std::unique_ptr<LevelManager> activeLevel)
    : State(stack, context),
      m_worldId(worldId),
      m_levelId(levelId),
      m_player(std::move(player)),
      m_activeLevel(std::move(activeLevel)) {
    if (!isValidStage(worldId, levelId)) {
        throw std::invalid_argument("GameState stage must be within W1_LV1 through W3_LV3");
    }
    if (!m_player || !m_activeLevel) {
        throw std::invalid_argument("GameState requires one player owner and one active level owner");
    }

    m_activeLevel->setPlayers(std::vector<PlayerManager*>{m_player.get()});
    bindLevelCallbacks();

    if (!m_activeLevel->isLoaded()) {
        m_loadError = m_activeLevel->getMapManager().getLastError();
        if (m_loadError.empty()) {
            m_loadError = "The selected level failed to load";
        }
        return;
    }
    if (!m_activeLevel->getWorldBounds()) {
        m_loadError = "The selected level has no valid world bounds";
        return;
    }
    m_ready = true;
}

GameState::~GameState() {
    clearLevelBindings();
}

void GameState::handleEvent(const sf::Event& event) {
    const auto* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) {
        return;
    }

    if (m_player->getPlayerId() == 1) {
        if (key->code == sf::Keyboard::Key::A) {
            m_facingDirection = -1.f;
        } else if (key->code == sf::Keyboard::Key::D) {
            m_facingDirection = 1.f;
        } else if (key->code == sf::Keyboard::Key::K) {
            (void)requestShoot(m_facingDirection);
        }
    } else if (key->code == sf::Keyboard::Key::Left) {
        m_facingDirection = -1.f;
    } else if (key->code == sf::Keyboard::Key::Right) {
        m_facingDirection = 1.f;
    } else if (key->code == sf::Keyboard::Key::Numpad0) {
        (void)requestShoot(m_facingDirection);
    }
}

void GameState::update(float deltaSeconds) {
    if (!m_ready || deltaSeconds <= 0.f) {
        return;
    }

    m_activeLevel->update(deltaSeconds);
    (void)m_events.flush();
}

void GameState::render(sf::RenderTarget& target) {
    if (!m_ready) {
        return;
    }

    const auto worldBounds = m_activeLevel->getWorldBounds();
    if (!worldBounds) {
        return;
    }

    const sf::View previousView = target.getView();
    const sf::FloatRect playerBounds = m_player->hitbox.getGlobalBounds();
    const auto gameplayView = buildClampedCamera(
        playerBounds.position + playerBounds.size / 2.f, previousView.getSize(), *worldBounds);
    if (!gameplayView) {
        return;
    }

    target.setView(*gameplayView);
    m_activeLevel->render(&target);
    target.setView(previousView);
}

bool GameState::isReady() const noexcept {
    return m_ready;
}

const std::string& GameState::loadError() const noexcept {
    return m_loadError;
}

PlayerManager& GameState::activePlayer() noexcept {
    return *m_player;
}

const PlayerManager& GameState::activePlayer() const noexcept {
    return *m_player;
}

LevelManager& GameState::activeLevel() noexcept {
    return *m_activeLevel;
}

const LevelManager& GameState::activeLevel() const noexcept {
    return *m_activeLevel;
}

GameEventMediator& GameState::events() noexcept {
    return m_events;
}

const GameEventMediator& GameState::events() const noexcept {
    return m_events;
}

bool GameState::requestShoot(float direction) {
    if (!m_ready) {
        return false;
    }
    const auto request = m_player->shoot(direction);
    return request && m_activeLevel->spawnProjectile(*request);
}

void GameState::bindLevelCallbacks() {
    m_activeLevel->setPlayerDeathCallback([this](PlayerManager& player) {
        (void)m_events.post(PlayerDiedEvent{nextEventId(), &player});
    });
    m_activeLevel->setLevelCompletedCallback([this] {
        (void)m_events.post(LevelCompletedEvent{nextEventId(), m_worldId, m_levelId});
    });
    m_activeLevel->setScoreChangedCallback([this](int delta) {
        (void)m_events.post(ScoreChangedEvent{nextEventId(), delta});
    });
    m_activeLevel->setCoinCollectedCallback([this](int delta) {
        (void)m_events.post(CoinCollectedEvent{nextEventId(), delta});
    });
    m_activeLevel->setLivesChangedCallback([this](int delta) {
        (void)m_events.post(LivesChangedEvent{nextEventId(), delta});
    });
}

void GameState::clearLevelBindings() noexcept {
    if (!m_activeLevel) {
        return;
    }
    m_activeLevel->setLevelCompletedCallback({});
    m_activeLevel->setPlayerDeathCallback({});
    m_activeLevel->setScoreChangedCallback({});
    m_activeLevel->setCoinCollectedCallback({});
    m_activeLevel->setLivesChangedCallback({});
    m_activeLevel->setPlayers({});
}

GameEventId GameState::nextEventId() noexcept {
    return m_nextEventId++;
}

std::optional<sf::View> GameState::buildClampedCamera(
    sf::Vector2f focus, sf::Vector2f viewportSize, const sf::FloatRect& worldBounds) {
    if (viewportSize.x <= 0.f || viewportSize.y <= 0.f ||
        worldBounds.size.x <= 0.f || worldBounds.size.y <= 0.f) {
        return std::nullopt;
    }

    sf::View view(sf::FloatRect({0.f, 0.f}, viewportSize));
    view.setCenter({clampedAxis(focus.x, viewportSize.x, worldBounds.position.x, worldBounds.size.x),
                    clampedAxis(focus.y, viewportSize.y, worldBounds.position.y, worldBounds.size.y)});
    return view;
}
