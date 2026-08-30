#include "Core/GameEventMediator.hpp"
#include "States/Base/GameState.hpp"
#include "States/Base/State.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

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
}

int main() {
    stateStackContract();
    mediatorContract();
    cameraContract();
    std::cout << "Person 1 state/event contracts passed\n";
    return 0;
}
