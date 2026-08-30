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
#include <filesystem>
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
    loaded = true;
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

void LevelManager::spawnRocket(sf::Vector2f position,
                               ProjectileTargetResolver targetResolver,
                               float speed, float lifetime) {
    addEntity(std::make_unique<Rocket>(position, std::move(targetResolver),
                                       speed, lifetime));
}

void LevelManager::spawnRocket(sf::Vector2f position, sf::Vector2f velocity,
                               float lifetime) {
    addEntity(std::make_unique<Rocket>(position, velocity, lifetime));
}

void LevelManager::update(float dt) {
    if (dt <= 0.f) return;
    updating = true;
    for (auto& entity : entities) {
        if (entity) entity->update(dt);
    }
    const auto worldBounds = getWorldBounds();
    for (PlayerManager* player : players) {
        if (player && !player->isDead()) {
            player->update(dt);
            physicsEngine.step(*player, blocks, dt);
            if (worldBounds) {
                physicsEngine.enforceHorizontalBounds(*player, *worldBounds);
            }
            if (worldBounds && player->hitbox.getGlobalBounds().position.y >
                                   worldBounds->position.y +
                                       worldBounds->size.y +
                                       MapFormat::TILE_SIZE) {
                killPlayer(*player);
            }
        }
    }
    for (Enemy* enemy : physicsEnemies) {
        if (enemy && !enemy->isDead()) {
            physicsEngine.step(*enemy, blocks, dt);
        }
    }
    enforceStagePatrols();
    resolveProjectileWorldCollisions();

    for (PlayerManager* player : players) {
        if (!player || player->isDead()) continue;
        for (Lava* lava : lavaHazards) {
            if (lava && player->hitbox.getGlobalBounds().findIntersection(
                            lava->hitbox.getGlobalBounds())) {
                if (lava->applyTo(*player) && playerDeathCallback) {
                    playerDeathCallback(*player);
                }
                break;
            }
        }
        if (player->isDead()) continue;
        for (Bullet* bullet : bullets) {
            if (bullet && bullet->isActive() &&
                player->hitbox.getGlobalBounds().findIntersection(
                    bullet->hitbox.getGlobalBounds())) {
                bullet->deactivate();
                damagePlayer(*player, 1);
            }
        }
        if (player->isDead()) continue;
        for (Rocket* rocket : rockets) {
            if (rocket && rocket->isActive() &&
                player->hitbox.getGlobalBounds().findIntersection(
                    rocket->hitbox.getGlobalBounds())) {
                rocket->deactivate();
                damagePlayer(*player, 1);
            }
        }
        if (player->isDead()) continue;
        for (WinFlag* flag : winFlags) {
            if (flag && player->hitbox.getGlobalBounds().findIntersection(
                            flag->hitbox.getGlobalBounds())) {
                flag->activate();
            }
        }
    }
    updating = false;
    flushPendingEntities();

    removeInactiveEntities();
    if (completionPending && !completionEmitted && levelCompletedCallback) {
        completionPending = false;
        completionEmitted = true;
        const LevelCallback callback = levelCompletedCallback;
        callback();
    }
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
    stagePatrols.clear();
    powerUps.clear();
    lavaHazards.clear();
    bullets.clear();
    rockets.clear();
    winFlags.clear();
    genericPhysicsParticipants.clear();
    blockCollisionParticipants.clear();
    completionPending = false;
    completionEmitted = false;
    loaded = false;
}

bool LevelManager::isLoaded() const noexcept { return loaded; }
std::optional<sf::FloatRect> LevelManager::getWorldBounds() const noexcept {
    if (!loaded) return std::nullopt;
    return mapManager.getWorldBounds();
}
const MapManager& LevelManager::getMapManager() const noexcept { return mapManager; }
const std::vector<std::unique_ptr<GameObject>>& LevelManager::getEntities() const noexcept { return entities; }
const std::vector<Block*>& LevelManager::getBlocks() const noexcept { return blocks; }
const std::vector<Enemy*>& LevelManager::getEnemies() const noexcept { return enemies; }
const std::vector<Enemy*>& LevelManager::getPhysicsEnemies() const noexcept { return physicsEnemies; }

Enemy* LevelManager::addStageEnemy(std::string_view type,
                                   sf::Vector2f position) {
    std::unique_ptr<Enemy> enemy =
        EntityFactory::createEnemy(std::string(type), position);
    if (!enemy) return nullptr;

    Enemy* view = enemy.get();
    const bool genericPhysics = type != "FlyingKoopa" &&
                                type != "PeteyPiranha";
    addEntity(std::move(enemy), genericPhysics);
    if (type == "Goomba" || type == "Koopa") {
        view->setFacingRight(true);
        stagePatrols.push_back({view, 32.f, 13416.f});
    }
    return view;
}

Enemy* LevelManager::addStageEnemy(std::string_view type,
                                   sf::Vector2f position,
                                   float patrolLeft, float patrolRight) {
    Enemy* enemy = addStageEnemy(type, position);
    if (!enemy || patrolLeft > patrolRight) return enemy;

    std::erase_if(stagePatrols, [enemy](const PatrolBinding& patrol) {
        return patrol.enemy == enemy;
    });
    enemy->setFacingRight(true);
    stagePatrols.push_back({enemy, patrolLeft, patrolRight});
    return enemy;
}

void LevelManager::addStageMovingBlock(sf::Vector2f position,
                                       std::string_view textureName,
                                       float speed, float distance) {
    addEntity(std::make_unique<MovingBlock>(
                  position, 1, distance, speed, textureName),
              false, true);
}

const std::vector<PlayerManager*>& LevelManager::getPlayers() const noexcept {
    return players;
}

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
                                               sf::Vector2f{
                                                   3.f * MapFormat::TILE_SIZE,
                                                   0.f},
                                               std::max(20.f, spawnInfo.parameter * 20.f));
        break;
    case MapObjectType::WinFlag:
        object = std::make_unique<WinFlag>(spawnInfo.position, [this] {
            if (loaded && !completionEmitted) completionPending = true;
        });
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
        const sf::Vector2f mapSize{
            static_cast<float>(spawnInfo.widthInTiles) * MapFormat::TILE_SIZE,
            static_cast<float>(spawnInfo.heightInTiles) * MapFormat::TILE_SIZE};
        if (auto* block = dynamic_cast<Block*>(object.get())) {
            block->setSizeBlock(mapSize);
        } else {
            object->setSize(mapSize);
        }
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
            if (const auto* block = dynamic_cast<const Block*>(entity.get())) {
                remove = !block->isExist();
            } else if (const auto* bullet = dynamic_cast<const Bullet*>(entity.get())) {
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
                std::erase_if(stagePatrols,
                    [&entity](const PatrolBinding& patrol) {
                        return patrol.enemy == entity.get();
                    });
            }
            return remove;
        }), entities.end());
    rebuildViews();
}

void LevelManager::enforceStagePatrols() {
    for (const PatrolBinding& patrol : stagePatrols) {
        if (!patrol.enemy || patrol.enemy->isDead()) continue;

        sf::Vector2f position = patrol.enemy->getPosition();
        if (position.x <= patrol.left) {
            position.x = patrol.left;
            patrol.enemy->setFacingRight(true);
        } else if (position.x >= patrol.right) {
            position.x = patrol.right;
            patrol.enemy->setFacingRight(false);
        }
        patrol.enemy->setPosition(position);
    }
}

void LevelManager::resolveProjectileWorldCollisions() {
    const auto worldBounds = getWorldBounds();
    const auto resolve = [this, &worldBounds](auto& projectile) {
        if (worldBounds && projectile.cullOutside(
                               *worldBounds, MapFormat::TILE_SIZE)) {
            return;
        }
        const sf::FloatRect bounds = projectile.hitbox.getGlobalBounds();
        const sf::FloatRect previous = projectile.getPreviousBounds();
        const float left = std::min(previous.position.x, bounds.position.x);
        const float top = std::min(previous.position.y, bounds.position.y);
        const float right = std::max(
            previous.position.x + previous.size.x,
            bounds.position.x + bounds.size.x);
        const float bottom = std::max(
            previous.position.y + previous.size.y,
            bounds.position.y + bounds.size.y);
        const sf::FloatRect motionBounds({left, top},
                                         {right - left, bottom - top});

        // ponytail: current projectile counts are tiny; replace the existing
        // linear nearby-block query with a spatial index if profiling says so.
        for (Block* block :
             physicsEngine.queryNearbyBlocks(motionBounds, blocks, 1)) {
            if (block && block->isExist() && projectile.deactivateOnWorldCollision(
                                                  block->hitbox.getGlobalBounds())) {
                return;
            }
        }
    };

    for (Bullet* bullet : bullets) {
        if (bullet && bullet->isActive()) resolve(*bullet);
    }
    for (Rocket* rocket : rockets) {
        if (rocket && rocket->isActive()) resolve(*rocket);
    }
}

bool LevelManager::damagePlayer(PlayerManager& player, int amount) {
    if (player.isDead()) return true;
    player.takeDamage(amount);
    if (!player.isDead()) return false;
    if (playerDeathCallback) playerDeathCallback(player);
    return true;
}

void LevelManager::killPlayer(PlayerManager& player) {
    if (player.isDead()) return;
    player.setDead(true);
    if (playerDeathCallback) playerDeathCallback(player);
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
      mapPath("assets/textures/LevelSketch_W" + std::to_string(worldNumber) +
              "_LV" + std::to_string(levelNumber) + ".png") {
    // ponytail: supports source-root and one-level build launches; use a
    // packaged asset root if binaries are nested more deeply later.
    if (!std::filesystem::exists(mapPath)) mapPath = "../" + mapPath;
    load(mapPath, activePlayers);
}

unsigned int ConfiguredLevel::getWorld() const noexcept { return world; }
unsigned int ConfiguredLevel::getLevel() const noexcept { return level; }
const std::string& ConfiguredLevel::getMapPath() const noexcept { return mapPath; }

