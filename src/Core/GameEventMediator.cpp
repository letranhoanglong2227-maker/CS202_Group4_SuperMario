#include "Core/GameEventMediator.hpp"

#include <algorithm>
#include <utility>

GameEventMediator::Subscription::Subscription(GameEventMediator& mediator, std::size_t listenerId,
                                              std::weak_ptr<void> lifetime)
    : m_mediator(&mediator), m_listenerId(listenerId), m_lifetime(std::move(lifetime)) {}

GameEventMediator::Subscription::~Subscription() {
    disconnect();
}

GameEventMediator::Subscription::Subscription(Subscription&& other) noexcept
    : m_mediator(std::exchange(other.m_mediator, nullptr)),
      m_listenerId(std::exchange(other.m_listenerId, 0)),
      m_lifetime(std::move(other.m_lifetime)) {}

GameEventMediator::Subscription& GameEventMediator::Subscription::operator=(Subscription&& other) noexcept {
    if (this != &other) {
        disconnect();
        m_mediator = std::exchange(other.m_mediator, nullptr);
        m_listenerId = std::exchange(other.m_listenerId, 0);
        m_lifetime = std::move(other.m_lifetime);
    }
    return *this;
}

void GameEventMediator::Subscription::disconnect() {
    if (m_mediator && !m_lifetime.expired()) {
        m_mediator->unsubscribe(m_listenerId);
    }
    m_mediator = nullptr;
    m_listenerId = 0;
    m_lifetime.reset();
}

bool GameEventMediator::Subscription::connected() const {
    return m_mediator && !m_lifetime.expired();
}

GameEventMediator::GameEventMediator() : m_lifetime(std::make_shared<int>(0)) {}

GameEventMediator::~GameEventMediator() {
    m_lifetime.reset();
}

GameEventMediator::Subscription GameEventMediator::subscribe(Listener listener) {
    const std::size_t listenerId = m_nextListenerId++;
    m_listeners.push_back({listenerId, std::move(listener), true});
    return Subscription(*this, listenerId, m_lifetime);
}

bool GameEventMediator::post(GameEvent event) {
    const GameEventId id = eventId(event);
    if (id == 0 || m_pendingIds.contains(id) || m_recentIds.contains(id)) {
        return false;
    }
    m_pendingIds.insert(id);
    m_pending.push_back(std::move(event));
    return true;
}

std::size_t GameEventMediator::flush() {
    if (m_dispatching || m_pending.empty()) {
        return 0;
    }

    std::deque<GameEvent> events;
    events.swap(m_pending);
    m_dispatching = true;
    std::size_t dispatched{};
    const auto finishDispatch = [this] {
        m_dispatching = false;
        std::erase_if(m_listeners, [](const ListenerEntry& entry) { return !entry.active; });
    };

    try {
        for (const GameEvent& event : events) {
            const GameEventId id = eventId(event);
            std::vector<std::size_t> listenerIds;
            listenerIds.reserve(m_listeners.size());
            for (const ListenerEntry& entry : m_listeners) {
                if (entry.active && entry.listener) {
                    listenerIds.push_back(entry.id);
                }
            }

            for (const std::size_t listenerId : listenerIds) {
                const auto current = std::find_if(m_listeners.begin(), m_listeners.end(),
                                                  [listenerId](const ListenerEntry& entry) {
                                                      return entry.id == listenerId && entry.active;
                                                  });
                if (current != m_listeners.end()) {
                    Listener callback = current->listener;
                    callback(event);
                }
            }

            m_pendingIds.erase(id);
            rememberDelivered(id);
            ++dispatched;
        }
    } catch (...) {
        for (const GameEvent& event : events) {
            m_pendingIds.erase(eventId(event));
        }
        finishDispatch();
        throw;
    }

    finishDispatch();
    return dispatched;
}

void GameEventMediator::clearPending() {
    m_pending.clear();
    m_pendingIds.clear();
}

std::size_t GameEventMediator::listenerCount() const {
    return static_cast<std::size_t>(std::count_if(m_listeners.begin(), m_listeners.end(),
                                                  [](const ListenerEntry& entry) { return entry.active; }));
}

std::size_t GameEventMediator::pendingCount() const {
    return m_pending.size();
}

void GameEventMediator::unsubscribe(std::size_t listenerId) {
    const auto listener = std::find_if(m_listeners.begin(), m_listeners.end(),
                                       [listenerId](const ListenerEntry& entry) {
                                           return entry.id == listenerId;
                                       });
    if (listener == m_listeners.end()) {
        return;
    }
    if (m_dispatching) {
        listener->active = false;
        listener->listener = {};
    } else {
        m_listeners.erase(listener);
    }
}

void GameEventMediator::rememberDelivered(GameEventId id) {
    // ponytail: bounded duplicate history; use producer-scoped generations if a duplicate can arrive >4096 events late.
    m_recentIds.insert(id);
    m_recentOrder.push_back(id);
    if (m_recentOrder.size() > RecentEventLimit) {
        m_recentIds.erase(m_recentOrder.front());
        m_recentOrder.pop_front();
    }
}

GameEventId GameEventMediator::eventId(const GameEvent& event) {
    return std::visit([](const auto& value) { return value.id; }, event);
}
