#include "Levels/Managers/LevelManager.hpp"

#include "Entities/EntityFactory.hpp"
#include "Entities/Base/Enemy.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Objects/Blocks/Block.hpp"
#include "Objects/Blocks/Brick.hpp"
#include "Objects/Blocks/CoinBlock.hpp"
#include "Objects/Blocks/MovingBlock.hpp"
#include "Objects/Blocks/MushroomBlock.hpp"
#include "Objects/Blocks/SolidBlock.hpp"
#include "Objects/Environment/Bullet.hpp"
#include "Objects/Environment/Cannon.hpp"
#include "Objects/Environment/Lava.hpp"
#include "Objects/Environment/Pipe.hpp"
#include "Objects/Environment/Rocket.hpp"
#include "Objects/Environment/Trampoline.hpp"
#include "Objects/Environment/WinFlag.hpp"
#include "Objects/Items/PowerUpObject.hpp"

#include <algorithm>
#include <utility>

LevelManager::LevelManager() : physicsEngine(980.f) {}

bool LevelManager::load(const std::string& mapPath,
                        const std::vector<PlayerManager*>& activePlayers) {
    clear();
    setPlayers(activePlayers);
    if (!mapManager.loadMap(mapPath)) return false;

    TextureBlockManager::setupTexture();
    for (const MapSpawnInfo& spawn : mapManager.getSpawns()) {
        constructSpawn(spawn);
    }
    flushPendingEntities();
    onMapLoaded();
    return true;
}

void LevelManager::setPlayers(const std::vector<PlayerManager*>& activePlayers) {
    players.clear();
    for (PlayerManager* player : activePlayers) {
        if (player && std::find(players.begin(), players.end(), player) == players.end()) {
            players.push_back(player);
        }
    }
}

void LevelManager::setLevelCompletedCallback(LevelCallback callback) {
    levelCompletedCallback = std::move(callback);
}

void LevelManager::setPlayerDeathCallback(PlayerCallback callback) {
    playerDeathCallback = std::move(callback);
}

void LevelManager::addEntity(std::unique_ptr<GameObject> entity,
                             bool participatesInGenericPhysics,
                             bool participatesInBlockCollisions) {
    if (!entity) return;
    if (participatesInGenericPhysics) {
        genericPhysicsParticipants.insert(entity.get());
    }
    if (participatesInBlockCollisions) {
        blockCollisionParticipants.insert(entity.get());
    }
    if (updating) {
        pendingEntities.push_back(std::move(entity));
        return;
    }
    registerEntity(*entity);
    entities.push_back(std::move(entity));
}

void LevelManager::update(float dt) {
    if (dt <= 0.f) return;
    updating = true;
    for (auto& entity : entities) {
        if (entity) entity->update(dt);
    }
    for (PlayerManager* player : players) {
        if (player && !player->isDead()) {
            player->update(dt);
            physicsEngine.step(*player, blocks, dt);
        }
    }
    for (Enemy* enemy : physicsEnemies) {
        if (enemy && !enemy->isDead()) {
            physicsEngine.step(*enemy, blocks, dt);
        }
    }

    for (PlayerManager* player : players) {
        if (!player || player->isDead()) continue;
        for (Lava* lava : lavaHazards) {
            if (lava && player->hitbox.getGlobalBounds().findIntersection(
                            lava->hitbox.getGlobalBounds())) {
                player->takeDamage(player->getHealth());
                if (playerDeathCallback) playerDeathCallback(*player);
            }
        }
        for (Bullet* bullet : bullets) {
            if (bullet && bullet->isActive() &&
                player->hitbox.getGlobalBounds().findIntersection(
                    bullet->hitbox.getGlobalBounds())) {
                bullet->deactivate();
                player->takeDamage(1);
                if (player->isDead() && playerDeathCallback) {
                    playerDeathCallback(*player);
                }
            }
        }
        for (Rocket* rocket : rockets) {
            if (rocket && rocket->isActive() &&
                player->hitbox.getGlobalBounds().findIntersection(
                    rocket->hitbox.getGlobalBounds())) {
                rocket->deactivate();
                player->takeDamage(1);
                if (player->isDead() && playerDeathCallback) {
                    playerDeathCallback(*player);
                }
            }
        }
        for (WinFlag* flag : winFlags) {
            if (flag && player->hitbox.getGlobalBounds().findIntersection(
                            flag->hitbox.getGlobalBounds())) {
                const bool wasActivated = flag->isActivated();
                flag->activate();
                if (!wasActivated && levelCompletedCallback) {
                    levelCompletedCallback();
                }
            }
        }
    }
    updating = false;
    flushPendingEntities();

    removeInactiveEntities();
}

void LevelManager::render(sf::RenderTarget* target) {
    if (!target) return;
    for (const auto& entity : entities) {
        if (entity) entity->render(target);
    }
    for (PlayerManager* player : players) {
        if (player) player->render(target);
    }
}

void LevelManager::clear() {
    pendingEntities.clear();
    entities.clear();
    blocks.clear();
    enemies.clear();
    physicsEnemies.clear();
    powerUps.clear();
    lavaHazards.clear();
    bullets.clear();
    rockets.clear();
    winFlags.clear();
    genericPhysicsParticipants.clear();
    blockCollisionParticipants.clear();
}

const MapManager& LevelManager::getMapManager() const noexcept { return mapManager; }
const std::vector<std::unique_ptr<GameObject>>& LevelManager::getEntities() const noexcept { return entities; }
const std::vector<Block*>& LevelManager::getBlocks() const noexcept { return blocks; }
const std::vector<Enemy*>& LevelManager::getEnemies() const noexcept { return enemies; }
const std::vector<Enemy*>& LevelManager::getPhysicsEnemies() const noexcept { return physicsEnemies; }

void LevelManager::constructSpawn(const MapSpawnInfo& spawnInfo) {
    std::unique_ptr<GameObject> object;
    bool participatesInGenericPhysics = false;
    auto adoptRawSpawn = [this](GameObject* spawned) {
        addEntity(std::unique_ptr<GameObject>(spawned));
    };
    auto adoptOwnedSpawn = [this](std::unique_ptr<GameObject> spawned) {
        addEntity(std::move(spawned));
    };

    switch (spawnInfo.type) {
    case MapObjectType::SolidGround:
    case MapObjectType::LavaBottom:
    case MapObjectType::CannonBody:
        object = std::make_unique<SolidBlock>();
        break;
    case MapObjectType::Brick:
        object = std::make_unique<Brick>(adoptRawSpawn);
        break;
    case MapObjectType::CoinBlock:
        object = std::make_unique<CoinBlock>(1, adoptRawSpawn);
        break;
    case MapObjectType::MushroomBlock:
        object = std::make_unique<MushroomBlock>(adoptRawSpawn);
        break;
    case MapObjectType::Pipe:
        object = std::make_unique<Pipe>(spawnInfo.position, spawnInfo.heightInTiles);
        break;
    case MapObjectType::Lava:
        object = std::make_unique<Lava>(spawnInfo.position);
        break;
    case MapObjectType::Trampoline:
        object = std::make_unique<Trampoline>(spawnInfo.position);
        break;
    case MapObjectType::Cannon:
        object = std::make_unique<Cannon>(spawnInfo.position, adoptOwnedSpawn);
        break;
    case MapObjectType::CloudPlatform:
        object = std::make_unique<MovingBlock>(spawnInfo.position,
                                               spawnInfo.widthInTiles,
                                               sf::Vector2f{96.f, 0.f},
                                               std::max(20.f, spawnInfo.parameter * 20.f));
        break;
    case MapObjectType::WinFlag:
        object = std::make_unique<WinFlag>(spawnInfo.position);
        break;
    case MapObjectType::Coin:
        object = EntityFactory::createItem("Coin", spawnInfo.position);
        break;
    case MapObjectType::Goomba:
        object = EntityFactory::createEnemy("Goomba", spawnInfo.position);
        participatesInGenericPhysics = true;
        break;
    case MapObjectType::Koopa:
        object = EntityFactory::createEnemy("Koopa", spawnInfo.position);
        participatesInGenericPhysics = true;
        break;
    case MapObjectType::FlyingKoopa:
        object = EntityFactory::createEnemy("FlyingKoopa", spawnInfo.position);
        break;
    case MapObjectType::Heriss:
        object = EntityFactory::createEnemy("Heriss", spawnInfo.position);
        participatesInGenericPhysics = true;
        break;
    case MapObjectType::PeteyPiranha:
        object = EntityFactory::createEnemy("PeteyPiranha", spawnInfo.position);
        break;
    case MapObjectType::Bowser:
        object = EntityFactory::createEnemy("Bowser", spawnInfo.position);
        participatesInGenericPhysics = true;
        break;
    case MapObjectType::Player1Spawn:
        if (PlayerManager* player = findPlayer(1)) player->setPosition(spawnInfo.position);
        return;
    case MapObjectType::Player2Spawn:
        if (PlayerManager* player = findPlayer(2)) player->setPosition(spawnInfo.position);
        return;
    }

    if (!object) return;
    object->setPosition(spawnInfo.position);
    if (object->getSize().x <= 0.f || object->getSize().y <= 0.f) {
        object->setSize({static_cast<float>(spawnInfo.widthInTiles) * MapFormat::TILE_SIZE,
                         static_cast<float>(spawnInfo.heightInTiles) * MapFormat::TILE_SIZE});
    }
    const bool participatesInBlockCollisions =
        dynamic_cast<Block*>(object.get()) != nullptr;
    addEntity(std::move(object), participatesInGenericPhysics,
              participatesInBlockCollisions);
}

void LevelManager::registerEntity(GameObject& entity) {
    if (auto* block = dynamic_cast<Block*>(&entity);
        block && blockCollisionParticipants.contains(&entity)) {
        blocks.push_back(block);
    }
    if (auto* enemy = dynamic_cast<Enemy*>(&entity)) {
        enemies.push_back(enemy);
        if (genericPhysicsParticipants.contains(&entity)) {
            physicsEnemies.push_back(enemy);
        }
    }
    if (auto* powerUp = dynamic_cast<PowerUpObject*>(&entity)) powerUps.push_back(powerUp);
    if (auto* lava = dynamic_cast<Lava*>(&entity)) lavaHazards.push_back(lava);
    if (auto* bullet = dynamic_cast<Bullet*>(&entity)) bullets.push_back(bullet);
    if (auto* rocket = dynamic_cast<Rocket*>(&entity)) rockets.push_back(rocket);
    if (auto* flag = dynamic_cast<WinFlag*>(&entity)) winFlags.push_back(flag);
}

void LevelManager::flushPendingEntities() {
    for (auto& entity : pendingEntities) {
        if (entity) {
            registerEntity(*entity);
            entities.push_back(std::move(entity));
        }
    }
    pendingEntities.clear();
}

void LevelManager::rebuildViews() {
    blocks.clear(); enemies.clear(); physicsEnemies.clear(); powerUps.clear();
    lavaHazards.clear(); bullets.clear(); rockets.clear(); winFlags.clear();
    for (auto& entity : entities) if (entity) registerEntity(*entity);
}

void LevelManager::removeInactiveEntities() {
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [this](const std::unique_ptr<GameObject>& entity) {
            bool remove = false;
            if (const auto* bullet = dynamic_cast<const Bullet*>(entity.get())) {
                remove = !bullet->isActive();
            } else if (const auto* rocket = dynamic_cast<const Rocket*>(entity.get())) {
                remove = !rocket->isActive();
            } else if (const auto* enemy = dynamic_cast<const Enemy*>(entity.get())) {
                remove = enemy->isDead();
            } else if (const auto* powerUp =
                           dynamic_cast<const PowerUpObject*>(entity.get())) {
                remove = !powerUp->exists();
            }
            if (remove) {
                genericPhysicsParticipants.erase(entity.get());
                blockCollisionParticipants.erase(entity.get());
            }
            return remove;
        }), entities.end());
    rebuildViews();
}

PlayerManager* LevelManager::findPlayer(int playerId) const {
    const auto found = std::find_if(players.begin(), players.end(),
        [playerId](const PlayerManager* player) {
            return player && player->getPlayerId() == playerId;
        });
    return found == players.end() ? nullptr : *found;
}

ConfiguredLevel::ConfiguredLevel(unsigned int worldNumber,
                                 unsigned int levelNumber,
                                 const std::vector<PlayerManager*>& activePlayers)
    : world(worldNumber), level(levelNumber),
      mapPath("assets/levels/LevelSketch_W" + std::to_string(worldNumber) +
              "_LV" + std::to_string(levelNumber) + ".png") {
    load(mapPath, activePlayers);
}

unsigned int ConfiguredLevel::getWorld() const noexcept { return world; }
unsigned int ConfiguredLevel::getLevel() const noexcept { return level; }
const std::string& ConfiguredLevel::getMapPath() const noexcept { return mapPath; }

