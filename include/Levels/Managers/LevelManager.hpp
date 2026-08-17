#pragma once

#include "Core/GameObject.hpp"
#include "Levels/Managers/MapManager.hpp"
#include "Physics/PhysicsEngine.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class Block;
class Bullet;
class Enemy;
class GameObject;
class Lava;
class PlayerManager;
class PowerUpObject;
class Rocket;
class WinFlag;

class LevelManager {
public:
    using LevelCallback = std::function<void()>;
    using PlayerCallback = std::function<void(PlayerManager&)>;

    LevelManager();
    virtual ~LevelManager() = default;
    bool load(const std::string& mapPath,
              const std::vector<PlayerManager*>& players = {});
    void setPlayers(const std::vector<PlayerManager*>& players);

    // TODO(Person1 integration): inject non-owning selected players before load.
    // Completion has no input; death receives the affected player by reference.
    // LevelManager invokes these callbacks and never owns the state-flow target.
    void setLevelCompletedCallback(LevelCallback callback);
    void setPlayerDeathCallback(PlayerCallback callback);
    void addEntity(std::unique_ptr<GameObject> entity,
                   bool participatesInGenericPhysics = false,
                   bool participatesInBlockCollisions = false);
    void update(float dt);
    void render(sf::RenderTarget* target);
    void clear();
    const MapManager& getMapManager() const noexcept;
    const std::vector<std::unique_ptr<GameObject>>& getEntities() const noexcept;
    const std::vector<Block*>& getBlocks() const noexcept;
    const std::vector<Enemy*>& getEnemies() const noexcept;
    const std::vector<Enemy*>& getPhysicsEnemies() const noexcept;

protected:
    virtual void onMapLoaded() {}

private:
    void constructSpawn(const MapSpawnInfo& spawn);
    void registerEntity(GameObject& entity);
    void flushPendingEntities();
    void rebuildViews();
    void removeInactiveEntities();
    PlayerManager* findPlayer(int playerId) const;

    MapManager mapManager;
    PhysicsEngine physicsEngine;
    std::vector<std::unique_ptr<GameObject>> entities;
    std::vector<std::unique_ptr<GameObject>> pendingEntities;
    std::vector<Block*> blocks;
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> physicsEnemies;
    std::vector<PowerUpObject*> powerUps;
    std::vector<Lava*> lavaHazards;
    std::vector<Bullet*> bullets;
    std::vector<Rocket*> rockets;
    std::vector<WinFlag*> winFlags;
    std::unordered_set<const GameObject*> genericPhysicsParticipants;
    std::unordered_set<const GameObject*> blockCollisionParticipants;
    std::vector<PlayerManager*> players;
    LevelCallback levelCompletedCallback;
    PlayerCallback playerDeathCallback;
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

