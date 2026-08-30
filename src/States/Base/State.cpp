#include "States/Base/State.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <stdexcept>
#include <utility>

State::State(StateStack& stack, StateContext context)
    : m_stack(&stack), m_context(context) {}

State::~State() = default;

bool State::requestPush(std::unique_ptr<State> state) {
    return m_stack->request({StateStack::Change::Push, std::move(state)});
}

bool State::requestPop() {
    return m_stack->request({StateStack::Change::Pop, nullptr});
}

bool State::requestReplace(std::unique_ptr<State> state) {
    return m_stack->request({StateStack::Change::Replace, std::move(state)});
}

bool State::requestQuit() {
    return m_stack->request({StateStack::Change::Quit, nullptr});
}

StateStack& State::stateStack() const {
    return *m_stack;
}

const StateContext& State::context() const {
    return m_context;
}

StateStack::StateStack(StateContext context) : m_context(context) {}

StateStack::~StateStack() = default;

void StateStack::pushInitial(std::unique_ptr<State> state) {
    if (!state || !m_states.empty()) {
        throw std::logic_error("StateStack requires one non-null initial state");
    }
    m_states.push_back(std::move(state));
}

void StateStack::handleEvent(const sf::Event& event) {
    if (m_states.empty() || m_quitRequested) {
        return;
    }
    m_states.back()->handleEvent(event);
    applyPendingChange();
}

void StateStack::update(float deltaSeconds) {
    if (m_states.empty() || m_quitRequested) {
        return;
    }
    m_states.back()->update(deltaSeconds);
    applyPendingChange();
}

void StateStack::render(sf::RenderTarget& target) {
    if (!m_states.empty() && !m_quitRequested) {
        m_states.back()->render(target);
    }
}

bool StateStack::empty() const {
    return m_states.empty();
}

std::size_t StateStack::size() const {
    return m_states.size();
}

bool StateStack::quitRequested() const {
    return m_quitRequested;
}

const StateContext& StateStack::context() const {
    return m_context;
}

bool StateStack::request(PendingChange change) {
    if (m_quitRequested || m_pendingChange.has_value()) {
        return false;
    }
    if ((change.change == Change::Push || change.change == Change::Replace) && !change.state) {
        return false;
    }
    m_pendingChange.emplace(std::move(change));
    return true;
}

void StateStack::applyPendingChange() {
    if (!m_pendingChange) {
        return;
    }

    PendingChange pending = std::move(*m_pendingChange);
    m_pendingChange.reset();

    switch (pending.change) {
    case Change::Push:
        m_states.push_back(std::move(pending.state));
        break;
    case Change::Pop:
        if (!m_states.empty()) {
            m_states.pop_back();
        }
        break;
    case Change::Replace:
        if (!m_states.empty()) {
            m_states.pop_back();
        }
        m_states.push_back(std::move(pending.state));
        break;
    case Change::Quit:
        m_states.clear();
        m_quitRequested = true;
        break;
    }
}
