#include "Core/GameEventMediator.hpp"
#include "Entities/EntityFactory.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Levels/Managers/LevelManager.hpp"
#include "States/Base/GameState.hpp"
#include "States/Base/State.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace {
class ProbeState final : public State {
public:
    enum class Action { None, Push, Pop, Replace, PushThenPop, Quit };

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
    explicit LifetimePlayer(bool& alive) : PlayerManager(1, "P1 runtime probe"), m_alive(&alive) {
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

    replacement->action = ProbeState::Action::Quit;
    stack.update(0.016f);
    assert(stack.quitRequested());
    assert(stack.empty());
    assert(destructionCount == 4);
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
    const auto wideLeft = GameState::buildClampedCamera(
        {-100.f, 300.f}, {800.f, 600.f}, sf::FloatRect({0.f, 0.f}, {2400.f, 960.f}));
    assert(wideLeft);
    assert(std::abs(wideLeft->getCenter().x - 400.f) < 0.001f);
    assert(std::abs(wideLeft->getCenter().y - 300.f) < 0.001f);

    const auto wideRight = GameState::buildClampedCamera(
        {3000.f, 900.f}, {800.f, 600.f}, sf::FloatRect({0.f, 0.f}, {2400.f, 960.f}));
    assert(wideRight);
    assert(std::abs(wideRight->getCenter().x - 2000.f) < 0.001f);
    assert(std::abs(wideRight->getCenter().y - 660.f) < 0.001f);

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
        assert(scoreDelta == 200);
        assert(coinDelta == 1);

        const sf::FloatRect world = *gameProbe->activeLevel().getWorldBounds();
        gameProbe->activePlayer().setPosition(
            {world.position.x, world.position.y + world.size.y + 65.f});
        stack.update(0.016f);
        stack.update(0.016f);
        assert(levelProbe->updateCalls == 3);
        assert(deathEvents == 1);
        assert(affectedPlayer == &gameProbe->activePlayer());

        sf::RenderTexture target({320u, 180u});
        const sf::View originalView = target.getView();
        stack.render(target);
        assert(levelProbe->renderCalls == 1);
        assert(target.getView().getCenter() == originalView.getCenter());
        assert(target.getView().getSize() == originalView.getSize());
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
}

int main() {
    stateStackContract();
    mediatorContract();
    cameraContract();
    gameStateRuntimeContract();
    std::cout << "Person 1 state/event contracts passed\n";
    return 0;
}
