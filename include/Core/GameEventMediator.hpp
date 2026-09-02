#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

class PlayerManager;

using GameEventId = std::uint64_t;

struct PlayerDiedEvent {
    GameEventId id{};
    const PlayerManager* player{}; // Non-owning; valid only while listeners run.
};

struct LevelCompletedEvent {
    GameEventId id{};
    int world{};
    int level{};
};

struct ScoreChangedEvent {
    GameEventId id{};
    int delta{};
};

struct CoinCollectedEvent {
    GameEventId id{};
    int delta{1};
};

struct LivesChangedEvent {
    GameEventId id{};
    int delta{};
};

enum class AudioCue {
    MenuMove,
    MenuConfirm,
    Jump,
    Coin,
    BrickBreak,
    PowerUp,
    Shoot,
    PlayerDeath,
    LevelComplete
};

struct AudioCueEvent {
    GameEventId id{};
    AudioCue cue{};
};

using GameEvent = std::variant<PlayerDiedEvent, LevelCompletedEvent, ScoreChangedEvent,
                               CoinCollectedEvent, LivesChangedEvent, AudioCueEvent>;

class GameEventMediator {
public:
    using Listener = std::function<void(const GameEvent&)>;

    class Subscription {
    public:
        Subscription() = default;
        ~Subscription();

        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;
        Subscription(Subscription&& other) noexcept;
        Subscription& operator=(Subscription&& other) noexcept;

        void disconnect();
        [[nodiscard]] bool connected() const;

    private:
        friend class GameEventMediator;
        Subscription(GameEventMediator& mediator, std::size_t listenerId, std::weak_ptr<void> lifetime);

        GameEventMediator* m_mediator{}; // Guarded by m_lifetime before use.
        std::size_t m_listenerId{};
        std::weak_ptr<void> m_lifetime;
    };

    GameEventMediator();
    ~GameEventMediator();

    GameEventMediator(const GameEventMediator&) = delete;
    GameEventMediator& operator=(const GameEventMediator&) = delete;

    [[nodiscard]] Subscription subscribe(Listener listener);
    [[nodiscard]] bool post(GameEvent event);
    std::size_t flush();
    void clearPending();

    [[nodiscard]] std::size_t listenerCount() const;
    [[nodiscard]] std::size_t pendingCount() const;

private:
    struct ListenerEntry {
        std::size_t id{};
        Listener listener;
        bool active{true};
    };

    void unsubscribe(std::size_t listenerId);
    void rememberDelivered(GameEventId eventId);
    [[nodiscard]] static GameEventId eventId(const GameEvent& event);

    static constexpr std::size_t RecentEventLimit = 4096;

    std::shared_ptr<void> m_lifetime;
    std::vector<ListenerEntry> m_listeners;
    std::deque<GameEvent> m_pending;
    std::unordered_set<GameEventId> m_pendingIds;
    std::deque<GameEventId> m_recentOrder;
    std::unordered_set<GameEventId> m_recentIds;
    std::size_t m_nextListenerId{1};
    bool m_dispatching{};
};
