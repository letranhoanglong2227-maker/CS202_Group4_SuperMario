#pragma once

#include "Core/GameObject.hpp"
#include "Core/GameEventMediator.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Physics/PhysicsEngine.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

class Block;
class Bullet;
class Enemy;
class Fireball;
class GameObject;
class Lava;
class PlayerManager;
class PowerUpObject;
class Rocket;
class WinFlag;
struct ProjectileSpawnRequest;

class LevelManager {
public:
    using LevelCallback = std::function<void()>;
    using PlayerCallback = std::function<void(PlayerManager&)>;
    using ValueCallback = std::function<void(int)>;
    using ProjectileTargetResolver =
        std::function<std::optional<sf::Vector2f>()>;
    using AudioCallback = std::function<void(AudioCue)>;

    explicit LevelManager(unsigned int visualWorld = 1);
    virtual ~LevelManager() = default;
    bool load(const std::string& mapPath,
              const std::vector<PlayerManager*>& players = {});
    void setPlayers(const std::vector<PlayerManager*>& players);

    // Completion has no input; death receives the affected player by reference.
    // LevelManager invokes these callbacks and never owns the state-flow target.
    void setLevelCompletedCallback(LevelCallback callback);
    void setPlayerDeathCallback(PlayerCallback callback);
    void setScoreChangedCallback(ValueCallback callback);
    void setCoinCollectedCallback(ValueCallback callback);
    void setLivesChangedCallback(ValueCallback callback);
    void setAudioCueCallback(AudioCallback callback);
    void addEntity(std::unique_ptr<GameObject> entity,
                   bool participatesInGenericPhysics = false,
                   bool participatesInBlockCollisions = false);
    void spawnRocket(sf::Vector2f position,
                     ProjectileTargetResolver targetResolver,
                     float speed = 140.f, float lifetime = 8.f);
    void spawnRocket(sf::Vector2f position, sf::Vector2f velocity,
                     float lifetime = 8.f);
    bool spawnProjectile(const ProjectileSpawnRequest& request);
    virtual void update(float dt);
    virtual void render(sf::RenderTarget* target);
    void clear();
    bool isLoaded() const noexcept;
    std::optional<sf::FloatRect> getWorldBounds() const noexcept;
    const MapManager& getMapManager() const noexcept;
    const std::vector<std::unique_ptr<GameObject>>& getEntities() const noexcept;
    const std::vector<Block*>& getBlocks() const noexcept;
    const std::vector<Enemy*>& getEnemies() const noexcept;
    const std::vector<Enemy*>& getPhysicsEnemies() const noexcept;

protected:
    virtual void onMapLoaded() {}
    Enemy* addStageEnemy(std::string_view type, sf::Vector2f position);
    Enemy* addStageEnemy(std::string_view type, sf::Vector2f position,
                         float patrolLeft, float patrolRight);
    void addStageMovingBlock(sf::Vector2f position,
                             std::string_view textureName,
                             float speed = 100.f, float distance = 300.f);
    const std::vector<PlayerManager*>& getPlayers() const noexcept;

private:
    struct PatrolBinding {
        Enemy* enemy;
        float left;
        float right;
    };

    void constructSpawn(const MapSpawnInfo& spawn);
    void registerEntity(GameObject& entity);
    void flushPendingEntities();
    void rebuildViews();
    void removeInactiveEntities();
    void enforceStagePatrols();
    void resolveBlockBumpEnemyCollisions();
    void resolveEnemyCollisions();
    void resolveProjectileWorldCollisions();
    bool damagePlayer(PlayerManager& player, int amount);
    void killPlayer(PlayerManager& player);
    PlayerManager* findPlayer(int playerId) const;

    MapManager mapManager;
    PhysicsEngine physicsEngine;
    unsigned int visualWorld{1};
    std::vector<std::unique_ptr<GameObject>> entities;
    std::vector<std::unique_ptr<GameObject>> pendingEntities;
    std::vector<Block*> blocks;
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> physicsEnemies;
    std::vector<PatrolBinding> stagePatrols;
    std::vector<PowerUpObject*> powerUps;
    std::vector<Lava*> lavaHazards;
    std::vector<Bullet*> bullets;
    std::vector<Rocket*> rockets;
    std::vector<Fireball*> fireballs;
    std::vector<WinFlag*> winFlags;
    std::unordered_set<const GameObject*> genericPhysicsParticipants;
    std::unordered_set<const GameObject*> blockCollisionParticipants;
    std::vector<PlayerManager*> players;
    LevelCallback levelCompletedCallback;
    PlayerCallback playerDeathCallback;
    ValueCallback scoreChangedCallback;
    ValueCallback coinCollectedCallback;
    ValueCallback livesChangedCallback;
    AudioCallback audioCueCallback;
    bool completionPending{false};
    bool completionEmitted{false};
    bool loaded{false};
    bool updating{false};
};

class ConfiguredLevel : public LevelManager {
public:
    ConfiguredLevel(unsigned int world, unsigned int level,
                    const std::vector<PlayerManager*>& players = {});
    unsigned int getWorld() const noexcept;
    unsigned int getLevel() const noexcept;
    const std::string& getMapPath() const noexcept;
private:
    unsigned int world;
    unsigned int level;
    std::string mapPath;
};

