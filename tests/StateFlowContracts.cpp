#include "Core/GameEventMediator.hpp"
#include "Core/MyApp.hpp"
#include "Core/UserData.hpp"
#include "Audio/AudioSystem.hpp"
#include "Entities/EntityFactory.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Levels/Managers/LevelManager.hpp"
#include "States/Base/GameState.hpp"
#include "States/Base/State.hpp"
#include "States/Menus/WinMenuState.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace {
class ProbeState final : public State {
public:
    enum class Action { None, Push, Pop, Replace, PushThenPop, PopToRoot, Quit };

    ProbeState(StateStack& stack, StateContext context, int& destructionCount)
        : State(stack, context), m_destructionCount(&destructionCount) {}

    ~ProbeState() override {
        ++*m_destructionCount;
    }

    void handleEvent(const sf::Event&) override {}

    void update(float) override {
        callbackReturned = false;
        switch (action) {
        case Action::None:
            break;
        case Action::Push:
        case Action::Replace: {
            auto next = std::make_unique<ProbeState>(stateStack(), context(), *m_destructionCount);
            if (createdState) {
                *createdState = next.get();
            }
            firstRequestAccepted = action == Action::Push ? requestPush(std::move(next))
                                                           : requestReplace(std::move(next));
            break;
        }
        case Action::Pop:
            firstRequestAccepted = requestPop();
            break;
        case Action::PopToRoot:
            firstRequestAccepted = requestPopToRoot();
            break;
        case Action::PushThenPop: {
            auto next = std::make_unique<ProbeState>(stateStack(), context(), *m_destructionCount);
            if (createdState) {
                *createdState = next.get();
            }
            firstRequestAccepted = requestPush(std::move(next));
            secondRequestAccepted = requestPop();
            break;
        }
        case Action::Quit:
            firstRequestAccepted = requestQuit();
            break;
        }
        callbackReturned = true;
        action = Action::None;
    }

    void render(sf::RenderTarget&) override {}

    Action action{Action::None};
    ProbeState** createdState{};
    bool firstRequestAccepted{};
    bool secondRequestAccepted{};
    bool callbackReturned{};

private:
    int* m_destructionCount;
};

class LifetimePlayer final : public PlayerManager {
public:
    explicit LifetimePlayer(bool& alive) : PlayerManager(1, "state runtime probe"), m_alive(&alive) {
        *m_alive = true;
    }

    ~LifetimePlayer() override {
        *m_alive = false;
    }

private:
    bool* m_alive;
};

class CountingLevel final : public LevelManager {
public:
    CountingLevel(bool& playerAliveAtLevelDestruction, bool& borrowedViewsCleared)
        : m_playerAliveAtLevelDestruction(&playerAliveAtLevelDestruction),
          m_borrowedViewsCleared(&borrowedViewsCleared) {}

    ~CountingLevel() override {
        *m_playerAliveAtLevelDestruction = *m_playerAlive;
        *m_borrowedViewsCleared = getPlayers().empty();
    }

    bool prepare(PlayerManager& player, bool& playerAlive) {
        m_playerAlive = &playerAlive;
        return load("assets/textures/LevelSketch_W1_LV1.png", {&player});
    }

    void update(float deltaSeconds) override {
        ++updateCalls;
        LevelManager::update(deltaSeconds);
    }

    void render(sf::RenderTarget* target) override {
        ++renderCalls;
        LevelManager::render(target);
    }

    int updateCalls{};
    int renderCalls{};

private:
    bool* m_playerAlive{};
    bool* m_playerAliveAtLevelDestruction;
    bool* m_borrowedViewsCleared;
};

void stateStackContract() {
    int destructionCount{};
    StateStack stack(StateContext{nullptr});

    auto initial = std::make_unique<ProbeState>(stack, stack.context(), destructionCount);
    ProbeState* first = initial.get();
    stack.pushInitial(std::move(initial));
    assert(stack.size() == 1);

    ProbeState* pushed{};
    first->createdState = &pushed;
    first->action = ProbeState::Action::Push;
    stack.update(0.016f);
    assert(first->callbackReturned);
    assert(first->firstRequestAccepted);
    assert(stack.size() == 2);
    assert(destructionCount == 0);

    pushed->action = ProbeState::Action::Pop;
    stack.update(0.016f);
    assert(stack.size() == 1);
    assert(destructionCount == 1);

    ProbeState* replacement{};
    first->createdState = &replacement;
    first->action = ProbeState::Action::Replace;
    stack.update(0.016f);
    assert(stack.size() == 1);
    assert(destructionCount == 2);

    ProbeState* duplicateChild{};
    replacement->createdState = &duplicateChild;
    replacement->action = ProbeState::Action::PushThenPop;
    stack.update(0.016f);
    assert(replacement->firstRequestAccepted);
    assert(!replacement->secondRequestAccepted);
    assert(stack.size() == 2);

    duplicateChild->action = ProbeState::Action::Pop;
    stack.update(0.016f);
    assert(stack.size() == 1);
    assert(destructionCount == 3);

    ProbeState* rootChild{};
    replacement->createdState = &rootChild;
    replacement->action = ProbeState::Action::Push;
    stack.update(0.016f);
    assert(stack.size() == 2);
    rootChild->action = ProbeState::Action::PopToRoot;
    stack.update(0.016f);
    assert(stack.size() == 1);
    assert(destructionCount == 4);

    replacement->action = ProbeState::Action::Quit;
    stack.update(0.016f);
    assert(stack.quitRequested());
    assert(stack.empty());
    assert(destructionCount == 5);
}

void mediatorContract() {
    GameEventMediator mediator;
    std::vector<int> callOrder;
    int observedScore{};

    auto first = mediator.subscribe([&](const GameEvent& event) {
        callOrder.push_back(1);
        if (const auto* score = std::get_if<ScoreChangedEvent>(&event)) {
            observedScore += score->delta;
        }
    });
    auto second = mediator.subscribe([&](const GameEvent&) { callOrder.push_back(2); });

    assert(mediator.post(ScoreChangedEvent{1, 100}));
    assert(!mediator.post(ScoreChangedEvent{1, 100}));
    assert(mediator.flush() == 1);
    assert((callOrder == std::vector<int>{1, 2}));
    assert(observedScore == 100);
    assert(!mediator.post(ScoreChangedEvent{1, 100}));

    first.disconnect();
    callOrder.clear();
    assert(mediator.post(CoinCollectedEvent{2, 1}));
    assert(mediator.flush() == 1);
    assert((callOrder == std::vector<int>{2}));

    GameEventMediator::Subscription selfRemoving;
    selfRemoving = mediator.subscribe([&](const GameEvent&) {
        callOrder.push_back(3);
        selfRemoving.disconnect();
    });
    callOrder.clear();
    assert(mediator.post(LivesChangedEvent{3, -1}));
    assert(mediator.flush() == 1);
    assert((callOrder == std::vector<int>{2, 3}));
    assert(mediator.listenerCount() == 1);

    second.disconnect();
    assert(mediator.listenerCount() == 0);
    assert(mediator.post(LevelCompletedEvent{4, 1, 1}));
    assert(mediator.flush() == 1); // No-listener delivery is safe.

    GameEventMediator::Subscription orphan;
    {
        GameEventMediator shortLived;
        orphan = shortLived.subscribe([](const GameEvent&) {});
        assert(orphan.connected());
    }
    assert(!orphan.connected());
    orphan.disconnect();

    GameEventMediator throwingMediator;
    auto throwing = throwingMediator.subscribe([](const GameEvent&) { throw std::runtime_error("listener"); });
    assert(throwingMediator.post(AudioCueEvent{9, AudioCue::Coin}));
    try {
        (void)throwingMediator.flush();
        assert(false && "listener exception must propagate");
    } catch (const std::runtime_error&) {
    }
    assert(throwingMediator.pendingCount() == 0);
    throwing.disconnect();
    assert(throwingMediator.post(AudioCueEvent{9, AudioCue::Coin}));
}

void cameraContract() {
    const auto group5Viewport = GameState::buildClampedCamera(
        {100.f, 100.f}, {1560.f, 960.f},
        sf::FloatRect({0.f, 0.f}, {8512.f, 960.f}));
    assert(group5Viewport);
    assert(group5Viewport->getSize() == sf::Vector2f(1560.f, 960.f));
    assert(std::abs(group5Viewport->getCenter().y - 480.f) < 0.001f);

    const auto wideLeft = GameState::buildClampedCamera(
        {-100.f, 300.f}, {800.f, 600.f}, sf::FloatRect({0.f, 0.f}, {2400.f, 960.f}));
    assert(wideLeft);
    assert(std::abs(wideLeft->getCenter().x - 400.f) < 0.001f);
    assert(std::abs(wideLeft->getCenter().y - 660.f) < 0.001f);

    const auto wideRight = GameState::buildClampedCamera(
        {3000.f, 900.f}, {800.f, 600.f}, sf::FloatRect({0.f, 0.f}, {2400.f, 960.f}));
    assert(wideRight);
    assert(std::abs(wideRight->getCenter().x - 2000.f) < 0.001f);
    assert(std::abs(wideRight->getCenter().y - 660.f) < 0.001f);

    const auto sameStageDifferentJumpHeight = GameState::buildClampedCamera(
        {1200.f, -500.f}, {800.f, 600.f},
        sf::FloatRect({0.f, 0.f}, {2400.f, 960.f}));
    assert(sameStageDifferentJumpHeight);
    assert(std::abs(sameStageDifferentJumpHeight->getCenter().y -
                    wideRight->getCenter().y) < 0.001f);

    const auto narrow = GameState::buildClampedCamera(
        {900.f, -100.f}, {800.f, 600.f}, sf::FloatRect({100.f, 40.f}, {500.f, 400.f}));
    assert(narrow);
    assert(std::abs(narrow->getCenter().x - 350.f) < 0.001f);
    assert(std::abs(narrow->getCenter().y - 240.f) < 0.001f);

    const auto equal = GameState::buildClampedCamera(
        {0.f, 0.f}, {800.f, 600.f}, sf::FloatRect({50.f, 70.f}, {800.f, 600.f}));
    assert(equal);
    assert(equal->getCenter() == sf::Vector2f(450.f, 370.f));

    assert(!GameState::buildClampedCamera(
        {0.f, 0.f}, {0.f, 600.f}, sf::FloatRect({0.f, 0.f}, {800.f, 600.f})));
}

void winMenuContinueContract() {
    UserData data("Continue Contract");
    data.unlockNextLevel(1, 2);
    assert(data.getCurrentWorld() == 1 && data.getCurrentLevel() == 1);

    StateStack stack(StateContext{nullptr, &data});
    stack.pushInitial(std::make_unique<WinMenuState>(
        stack, stack.context(), 1, 1));
    stack.handleEvent(sf::Event(
        sf::Event::KeyPressed{sf::Keyboard::Key::Enter}));

    assert(stack.size() == 1); // Win was replaced, not stacked over the game.
    assert(data.getCurrentWorld() == 1 && data.getCurrentLevel() == 2);
    stack.handleEvent(sf::Event(
        sf::Event::KeyPressed{sf::Keyboard::Key::Escape}));
    assert(stack.size() == 2); // The replacement is a playable GameState.
}

void gameStateRuntimeContract() {
    bool playerAlive{};
    bool playerAliveAtLevelDestruction{};
    bool borrowedViewsCleared{};

    {
        StateStack stack(StateContext{nullptr});
        auto player = std::make_unique<LifetimePlayer>(playerAlive);
        auto level = std::make_unique<CountingLevel>(
            playerAliveAtLevelDestruction, borrowedViewsCleared);
        CountingLevel* levelProbe = level.get();
        assert(level->prepare(*player, playerAlive));

        auto game = std::make_unique<GameState>(
            stack, stack.context(), 1, 1, std::move(player), std::move(level));
        GameState* gameProbe = game.get();
        assert(gameProbe->isReady());
        assert(gameProbe->loadError().empty());

        int deathEvents{};
        int scoreDelta{};
        int coinDelta{};
        const PlayerManager* affectedPlayer{};
        auto subscription = gameProbe->events().subscribe([&](const GameEvent& event) {
            if (const auto* death = std::get_if<PlayerDiedEvent>(&event)) {
                ++deathEvents;
                affectedPlayer = death->player;
            } else if (const auto* score = std::get_if<ScoreChangedEvent>(&event)) {
                scoreDelta += score->delta;
            } else if (const auto* coin = std::get_if<CoinCollectedEvent>(&event)) {
                coinDelta += coin->delta;
            }
        });

        const sf::Vector2f playerPosition =
            gameProbe->activePlayer().hitbox.getGlobalBounds().position;
        gameProbe->activeLevel().addEntity(EntityFactory::createItem("Coin", playerPosition));

        gameProbe->activePlayer().setFire(true);
        const std::size_t entityCount = gameProbe->activeLevel().getEntities().size();
        gameProbe->handleEvent(sf::Event(
            sf::Event::KeyPressed{sf::Keyboard::Key::A}));
        gameProbe->handleEvent(sf::Event(
            sf::Event::KeyPressed{sf::Keyboard::Key::K}));
        assert(gameProbe->activeLevel().getEntities().size() == entityCount + 1);
        gameProbe->handleEvent(sf::Event(
            sf::Event::KeyPressed{sf::Keyboard::Key::K}));
        assert(gameProbe->activeLevel().getEntities().size() == entityCount + 1);

        stack.pushInitial(std::move(game));
        stack.update(0.016f);
        assert(levelProbe->updateCalls == 1);
        assert(scoreDelta == 0);
        assert(coinDelta == 1);

        sf::RenderTexture target({320u, 180u});
        const sf::View originalView = target.getView();
        stack.render(target);
        assert(levelProbe->renderCalls == 1);
        assert(target.getView().getCenter() == originalView.getCenter());
        assert(target.getView().getSize() == originalView.getSize());

        const sf::FloatRect world = *gameProbe->activeLevel().getWorldBounds();
        gameProbe->activePlayer().setPosition(
            {world.position.x, world.position.y + world.size.y + 65.f});
        stack.update(0.016f);
        stack.update(0.016f);
        assert(levelProbe->updateCalls == 2);
        assert(stack.size() == 2); // Death menu pauses the game below it.
        assert(deathEvents == 1);
        assert(affectedPlayer == &gameProbe->activePlayer());
        subscription.disconnect();
    }

    assert(playerAliveAtLevelDestruction);
    assert(borrowedViewsCleared);
    assert(!playerAlive);

    StateStack failedStack(StateContext{nullptr});
    auto failedGame = std::make_unique<GameState>(
        failedStack, failedStack.context(), 1, 1,
        std::make_unique<PlayerManager>(), std::make_unique<LevelManager>());
    GameState* failedProbe = failedGame.get();
    failedStack.pushInitial(std::move(failedGame));
    assert(!failedProbe->isReady());
    assert(!failedProbe->loadError().empty());
    failedStack.update(0.016f);
}

void userDataPersistenceContract() {
    const std::filesystem::path path = "UserDataContract.save";
    const std::filesystem::path leaderboardPath = "LeaderboardContract.save";
    UserData original("Player One", 5);
    original.add_score(1234);
    original.add_coins(17);
    original.setCharacter(1);
    original.setPlayerForm(2);
    original.unlockNextLevel(3, 3); // Unlock progress remains sequential.
    original.setCurrentLevel(3, 3);
    assert(original.getMaxUnlockedWorld() == 1 &&
           original.getMaxUnlockedLevel() == 1);
    assert(original.getCurrentWorld() == 3 && original.getCurrentLevel() == 3);
    original.unlockNextLevel(1, 2);
    original.unlockNextLevel(1, 3);
    original.unlockNextLevel(2, 1);
    original.unlockNextLevel(2, 2);
    original.setCurrentLevel(2, 2);

    std::string error;
    assert(original.saveTo(path, &error));
    assert(error.empty());

    UserData loaded;
    assert(loaded.loadFrom(path, &error));
    assert(loaded.getPlayerName() == "Player One");
    assert(loaded.getLives() == 5);
    assert(loaded.getScore() == 1234);
    assert(loaded.getNumberOfCoins() == 17);
    assert(loaded.getCurrentWorld() == 2 && loaded.getCurrentLevel() == 2);
    assert(loaded.getMaxUnlockedWorld() == 2 &&
           loaded.getMaxUnlockedLevel() == 2);
    assert(loaded.getCharacter() == 1);
    assert(loaded.getPlayerForm() == 2);
    std::filesystem::remove(path);

    {
        std::ofstream legacy(path, std::ios::trunc);
        legacy << "GROUP4_SAVE_V1\n\"Legacy\"\n3 400 5 1 2 1 2 0\n";
    }
    UserData legacy;
    assert(legacy.loadFrom(path, &error));
    assert(legacy.getPlayerName() == "Legacy");
    assert(legacy.getPlayerForm() == 0);
    std::filesystem::remove(path);

    {
        std::ofstream malformed(path, std::ios::trunc);
        malformed << "GROUP4_SAVE_V2\n\"Player\"\n3 0 0 1 1 1 1 0 0 EXTRA\n";
    }
    assert(!legacy.loadFrom(path, &error));
    assert(!error.empty());
    std::filesystem::remove(path);

    for (int index = 0; index < 12; ++index) {
        UserData candidate("Player " + std::to_string(index), 3);
        candidate.add_score(index * 100);
        assert(candidate.updateLeaderboard(leaderboardPath, &error));
    }
    UserData improved("Player 5", 3);
    improved.add_score(5000);
    improved.unlockNextLevel(1, 2);
    improved.unlockNextLevel(1, 3);
    improved.unlockNextLevel(2, 1);
    improved.unlockNextLevel(2, 2);
    improved.unlockNextLevel(2, 3);
    improved.unlockNextLevel(3, 1);
    improved.unlockNextLevel(3, 2);
    improved.setCurrentLevel(3, 2);
    assert(improved.updateLeaderboard(leaderboardPath, &error));

    UserData worseAttempt("Player 5", 3);
    worseAttempt.add_score(100);
    worseAttempt.setCurrentLevel(1, 2);
    assert(worseAttempt.updateLeaderboard(leaderboardPath, &error));

    const auto leaderboard = UserData::loadLeaderboard(leaderboardPath, &error);
    assert(error.empty());
    assert(leaderboard.size() == 10);
    assert(leaderboard.front().playerName == "Player 5");
    assert(leaderboard.front().score == 5000);
    assert(leaderboard.front().world == 3 && leaderboard.front().level == 2);
    assert(std::count_if(
               leaderboard.begin(), leaderboard.end(),
               [](const LeaderboardEntry& entry) {
                   return entry.playerName == "Player 5";
               }) == 1);
    for (std::size_t index = 1; index < leaderboard.size(); ++index)
        assert(leaderboard[index - 1].score >= leaderboard[index].score);
    std::filesystem::remove(leaderboardPath);
}
}

int main() {
    assert(MyApp::fitWindowToDesktop({1920u, 1080u}) ==
           sf::Vector2u(1560u, 960u));
    assert(MyApp::fitWindowToDesktop({1366u, 768u}) ==
           sf::Vector2u(1122u, 691u));

    const sf::View compactView =
        MyApp::buildLetterboxedView({1366u, 768u});
    assert(compactView.getSize() == sf::Vector2f(1560.f, 960.f));
    assert(compactView.getCenter() == sf::Vector2f(640.f, 360.f));
    assert(compactView.getViewport().size.x < 1.f);
    assert(compactView.getViewport().size.y == 1.f);

    static_assert(!std::is_default_constructible_v<AudioSystem>);
    static_assert(!std::is_copy_constructible_v<AudioSystem>);
    AudioSystem& audio = AudioSystem::instance();
    assert(&audio == &AudioSystem::instance());
    assert(audio.areEffectsEnabled());
    audio.setEffectsEnabled(false);
    assert(!audio.areEffectsEnabled() && audio.playCue(AudioCue::Coin));
    audio.setEffectsEnabled(true);
    assert(audio.areEffectsEnabled());
    stateStackContract();
    mediatorContract();
    cameraContract();
    winMenuContinueContract();
    gameStateRuntimeContract();
    userDataPersistenceContract();
    std::cout << "State and event contracts passed\n";
    return 0;
}
