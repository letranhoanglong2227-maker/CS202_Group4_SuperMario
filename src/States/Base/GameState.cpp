#include "States/Base/GameState.hpp"

#include "Core/AssetLocator.hpp"
#include "Audio/AudioSystem.hpp"
#include "Core/UserData.hpp"
#include "Entities/EntityFactory.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Levels/LevelFactory.hpp"
#include "Levels/Managers/LevelManager.hpp"
#include "States/Menus/DeathMenuState.hpp"
#include "States/Menus/PauseMenuState.hpp"
#include "States/Menus/WinMenuState.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>
#include <iostream>

namespace {
float clampedAxis(float focus, float viewportSize, float worldStart, float worldSize) {
    if (worldSize <= viewportSize) {
        return worldStart + worldSize / 2.f;
    }

    const float halfViewport = viewportSize / 2.f;
    return std::clamp(focus, worldStart + halfViewport, worldStart + worldSize - halfViewport);
}

float bottomFramedAxis(float viewportSize, float worldStart, float worldSize) {
    if (worldSize <= viewportSize) return worldStart + worldSize / 2.f;
    return worldStart + worldSize - viewportSize / 2.f;
}

bool isValidStage(int worldId, int levelId) {
    return worldId >= 1 && worldId <= 3 && levelId >= 1 && levelId <= 3;
}

void syncPlayerForm(UserData* data, const PlayerManager* player) {
    if (!data || !player) return;
    data->setPlayerForm(player->isFire() ? 2 : player->isBig() ? 1 : 0);
}

void saveSession(UserData* data, const PlayerManager* player) {
    if (!data) return;
    syncPlayerForm(data, player);
    std::string error;
    const auto directory = AssetLocator::executableDirectory();
    if (!data->saveTo(directory / "savegame.txt", &error)) {
        std::cerr << "Could not save session: " << error << '\n';
        return;
    }
    if (!data->updateLeaderboard(
            AssetLocator::executableDirectory() / "leaderboard.txt", &error))
        std::cerr << "Could not update leaderboard: " << error << '\n';
}

std::string backgroundAsset(int worldId, int levelId) {
    if (worldId == 1)
        return levelId == 2
            ? "assets/textures/basicnight_background.png"
            : "assets/textures/basic_background.png";
    if (worldId == 2)
        return levelId == 2
            ? "assets/textures/snownight_background.png"
            : "assets/textures/snow_background.png";
    return "assets/textures/gray_background.png";
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
    m_runtimeSubscription = m_events.subscribe(
        [this](const GameEvent& event) { handleRuntimeEvent(event); });

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
    const auto backgroundPath = AssetLocator::find(
        backgroundAsset(worldId, levelId));
    m_backgroundLoaded = backgroundPath &&
                         m_backgroundTexture.loadFromFile(*backgroundPath);
    if (m_backgroundLoaded) {
        m_backgroundTexture.setRepeated(true);
    }
    m_ready = true;
    if (context.audio) (void)context.audio->playWorldMusic(worldId);
}

GameState::~GameState() {
    m_runtimeSubscription.disconnect();
    clearLevelBindings();
}

void GameState::handleEvent(const sf::Event& event) {
    if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>();
        click && click->button == sf::Mouse::Button::Left &&
        context().window && m_ready) {
        const auto worldBounds = m_activeLevel->getWorldBounds();
        const sf::FloatRect playerBounds = m_player->hitbox.getGlobalBounds();
        const auto gameplayView = worldBounds
            ? buildClampedCamera(playerBounds.position + playerBounds.size / 2.f,
                                 context().window->getView().getSize(),
                                 *worldBounds)
            : std::nullopt;
        if (gameplayView) {
            const sf::Vector2f target = context().window->mapPixelToCoords(
                click->position, *gameplayView);
            if (requestShoot(target - m_player->getCenter(), 750.f)) {
                (void)m_events.post(
                    AudioCueEvent{nextEventId(), AudioCue::Shoot});
            }
        }
        return;
    }

    const auto* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) {
        return;
    }

    if (key->code == sf::Keyboard::Key::Escape) {
        (void)requestPush(std::make_unique<PauseMenuState>(
            stateStack(), context(), [this] { return restartLevel(); }));
        return;
    }

    if (key->code == sf::Keyboard::Key::A ||
        key->code == sf::Keyboard::Key::Left) {
        m_facingDirection = -1.f;
    } else if (key->code == sf::Keyboard::Key::D ||
               key->code == sf::Keyboard::Key::Right) {
        m_facingDirection = 1.f;
    } else if (key->code == sf::Keyboard::Key::K ||
               key->code == sf::Keyboard::Key::Numpad0) {
        if (requestShoot(m_facingDirection)) {
            (void)m_events.post(AudioCueEvent{nextEventId(), AudioCue::Shoot});
        }
    } else if ((key->code == sf::Keyboard::Key::W ||
                key->code == sf::Keyboard::Key::Up ||
                key->code == sf::Keyboard::Key::J ||
                key->code == sf::Keyboard::Key::Numpad1 ||
                key->code == sf::Keyboard::Key::Space) &&
               m_player->canJump()) {
        (void)m_events.post(AudioCueEvent{nextEventId(), AudioCue::Jump});
    }
}

void GameState::update(float deltaSeconds) {
    if (!m_ready || deltaSeconds <= 0.f) {
        return;
    }

    m_activeLevel->update(deltaSeconds);
    (void)m_events.flush();
    syncPlayerForm(context().userData, m_player.get());
    m_hud.update(deltaSeconds);
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
    if (m_backgroundLoaded) {
        const sf::Vector2u textureSize = m_backgroundTexture.getSize();
        const float scale = worldBounds->size.y /
            static_cast<float>(textureSize.y);
        sf::Sprite background(m_backgroundTexture);
        background.setTextureRect(sf::IntRect(
            {0, 0},
            {static_cast<int>(std::ceil(worldBounds->size.x / scale)),
             static_cast<int>(textureSize.y)}));
        background.setPosition(worldBounds->position);
        background.setScale({scale, scale});
        target.draw(background);
    } else {
        sf::RectangleShape fallback(worldBounds->size);
        fallback.setPosition(worldBounds->position);
        fallback.setFillColor(sf::Color(92, 184, 232));
        target.draw(fallback);
    }
    m_activeLevel->render(&target);
    target.setView(previousView);
    m_hud.render(target, context().userData, m_worldId, m_levelId,
                 m_player->getHealth(), m_player->isBig() || m_player->isFire());
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
    return requestShoot({direction, 0.f}, 400.f);
}

bool GameState::requestShoot(sf::Vector2f direction, float speed) {
    if (!m_ready) {
        return false;
    }
    auto request = m_player->shoot(direction.x);
    if (!request) return false;
    request->direction = direction;
    request->speed = speed;
    return m_activeLevel->spawnProjectile(*request);
}

bool GameState::restartLevel() {
    const std::string character = m_player &&
            m_player->getCharacterName() == "Luigi"
        ? "Luigi"
        : "Mario";
    auto player = EntityFactory::createPlayer(character);
    if (!player) return false;
    auto level = createConfiguredLevel(
        m_worldId, m_levelId, {player.get()});
    if (!level || !level->isLoaded() || !level->getWorldBounds()) return false;

    clearLevelBindings();
    m_activeLevel = std::move(level);
    m_player = std::move(player);
    m_activeLevel->setPlayers({m_player.get()});
    bindLevelCallbacks();
    m_loadError.clear();
    m_ready = true;
    m_endMenuOpen = false;
    m_hud.resetTimer();
    return true;
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
    m_activeLevel->setAudioCueCallback([this](AudioCue cue) {
        (void)m_events.post(AudioCueEvent{nextEventId(), cue});
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
    m_activeLevel->setAudioCueCallback({});
    m_activeLevel->setPlayers({});
}

void GameState::handleRuntimeEvent(const GameEvent& event) {
    UserData* data = context().userData;
    if (const auto* audio = std::get_if<AudioCueEvent>(&event)) {
        if (context().audio) (void)context().audio->playCue(audio->cue);
        return;
    }
    if (const auto* score = std::get_if<ScoreChangedEvent>(&event)) {
        if (data) data->add_score(score->delta);
        return;
    }
    if (const auto* coin = std::get_if<CoinCollectedEvent>(&event)) {
        if (data) data->add_coins(coin->delta);
        return;
    }
    if (const auto* lives = std::get_if<LivesChangedEvent>(&event)) {
        if (data) {
            if (lives->delta >= 0) data->add_lives(lives->delta);
            else data->reduce_lives(-lives->delta);
        }
        return;
    }
    if (std::holds_alternative<PlayerDiedEvent>(event)) {
        if (m_endMenuOpen) return;
        m_endMenuOpen = true;
        if (context().audio) (void)context().audio->playCue(AudioCue::PlayerDeath);
        if (data) data->reduce_lives();
        saveSession(data, m_player.get());
        (void)requestPush(std::make_unique<DeathMenuState>(
            stateStack(), context(), [this] { return restartLevel(); }));
        return;
    }
    if (std::holds_alternative<LevelCompletedEvent>(event)) {
        if (m_endMenuOpen) return;
        m_endMenuOpen = true;
        if (context().audio) (void)context().audio->playCue(AudioCue::LevelComplete);
        if (data) {
            if (m_worldId != 3 || m_levelId != 3) {
                int nextWorld = m_worldId;
                int nextLevel = m_levelId + 1;
                if (nextLevel > 3) {
                    nextLevel = 1;
                    ++nextWorld;
                }
                data->unlockNextLevel(nextWorld, nextLevel);
                data->setCurrentLevel(nextWorld, nextLevel);
            }
            saveSession(data, m_player.get());
        }
        // Replace the finished game so WinMenu can replace itself with the
        // next stage without leaving a stale GameState underneath it.
        (void)requestReplace(std::make_unique<WinMenuState>(
            stateStack(), context(), m_worldId, m_levelId));
    }
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
    view.setCenter(
        {clampedAxis(focus.x, viewportSize.x, worldBounds.position.x,
                     worldBounds.size.x),
         bottomFramedAxis(viewportSize.y, worldBounds.position.y,
                          worldBounds.size.y)});
    return view;
}
