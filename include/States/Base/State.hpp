#pragma once

#include <SFML/Window/Event.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

namespace sf {
class RenderTarget;
class RenderWindow;
}

class StateStack;

struct StateContext {
    sf::RenderWindow* window{}; // Non-owning; MyApp owns the window.
};

class State {
public:
    State(StateStack& stack, StateContext context);
    virtual ~State();

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaSeconds) = 0;
    virtual void render(sf::RenderTarget& target) = 0;

protected:
    [[nodiscard]] bool requestPush(std::unique_ptr<State> state);
    [[nodiscard]] bool requestPop();
    [[nodiscard]] bool requestReplace(std::unique_ptr<State> state);
    [[nodiscard]] bool requestQuit();

    [[nodiscard]] StateStack& stateStack() const;
    [[nodiscard]] const StateContext& context() const;

private:
    StateStack* m_stack; // Non-owning; the stack owns every State.
    StateContext m_context;
};

class StateStack {
public:
    explicit StateStack(StateContext context);
    ~StateStack();

    StateStack(const StateStack&) = delete;
    StateStack& operator=(const StateStack&) = delete;

    void pushInitial(std::unique_ptr<State> state);
    void handleEvent(const sf::Event& event);
    void update(float deltaSeconds);
    void render(sf::RenderTarget& target);

    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool quitRequested() const;
    [[nodiscard]] const StateContext& context() const;

private:
    friend class State;

    enum class Change { Push, Pop, Replace, Quit };

    struct PendingChange {
        Change change;
        std::unique_ptr<State> state;
    };

    [[nodiscard]] bool request(PendingChange change);
    void applyPendingChange();

    StateContext m_context;
    std::vector<std::unique_ptr<State>> m_states;
    std::optional<PendingChange> m_pendingChange;
    bool m_quitRequested{};
};
