#include "Entities/Base/Enemy.hpp"
#include "Entities/Players/Mario.hpp"
#include "Levels/Managers/LevelManager.hpp"
#include "Objects/Blocks/Block.hpp"
#include "Objects/Blocks/Brick.hpp"
#include "Objects/Blocks/BrickFragment.hpp"
#include "Objects/Environment/Bullet.hpp"
#include "Objects/Environment/Cannon.hpp"
#include "Objects/Environment/Rocket.hpp"
#include "Physics/PhysicsEngine.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace {
class TestEnemy final : public Enemy {
public:
    explicit TestEnemy(sf::Vector2f position) {
        setPosition(position);
        setSize({32.f, 32.f});
        movementComponent =
            std::make_unique<MovementComponent>(400.f, 0.f, 0.f);
    }

    void update(float dt) override { LivingEntity::update(dt); }
};

class TestBlock final : public Block {
public:
    TestBlock(sf::Vector2f position, sf::Vector2f blockSize) {
        setPosition(position);
        setSize(blockSize);
    }

    void initSpritesSheet() override {}
    void reactToCollision(int side) override { lastCollisionSide = side; }
    void update(float) override {}

    int lastCollisionSide{-1};
};

bool nearlyEqual(float lhs, float rhs, float tolerance = 0.01f) {
    return std::abs(lhs - rhs) <= tolerance;
}

bool check(bool condition, const std::string& name) {
    std::cout << (condition ? "PASS " : "FAIL ") << name << '\n';
    return condition;
}
}

int main() {
    bool passed = true;

    {
        LevelManager level;
        auto generic = std::make_unique<TestEnemy>(sf::Vector2f{0.f, 0.f});
        auto custom = std::make_unique<TestEnemy>(sf::Vector2f{64.f, 0.f});
        TestEnemy* genericView = generic.get();
        TestEnemy* customView = custom.get();
        level.addEntity(std::move(generic), true);
        level.addEntity(std::move(custom), false);
        level.update(0.05f);

        passed &= check(level.getEnemies().size() == 2,
                        "all enemies remain owned and updated");
        passed &= check(level.getPhysicsEnemies().size() == 1,
                        "only opted-in enemy enters generic physics view");
        passed &= check(genericView->getPosition().y > 0.f,
                        "generic enemy receives gravity integration");
        passed &= check(nearlyEqual(customView->getPosition().y, 0.f),
                        "custom-motion enemy avoids generic integration");
    }

    {
        PhysicsEngine physics;
        TestEnemy actor({0.f, 20.f});
        TestBlock floor({0.f, 64.f}, {64.f, 32.f});
        actor.getMovementComponent()->setVelocity(0.f, 300.f);
        const CollisionInfo collision = physics.step(actor, {&floor}, 0.05f);
        passed &= check(collision.grounded &&
                            nearlyEqual(actor.getPosition().y, 32.f),
                        "floor collision resolves and grounds actor");
        passed &= check(floor.lastCollisionSide == 1,
                        "floor receives top-side reaction");
    }

    {
        PhysicsEngine physics(0.f);
        TestEnemy actor({20.f, 0.f});
        TestBlock wall({64.f, 0.f}, {32.f, 96.f});
        actor.getMovementComponent()->setVelocity(400.f, 0.f);
        const CollisionInfo collision = physics.step(actor, {&wall}, 0.05f);
        passed &= check(collision.wallHit &&
                            nearlyEqual(actor.getPosition().x, 32.f),
                        "wall collision resolves on X axis");
    }

    {
        PhysicsEngine physics(0.f);
        TestEnemy actor({0.f, 48.f});
        TestBlock ceiling({0.f, 0.f}, {64.f, 32.f});
        actor.getMovementComponent()->setVelocity(0.f, -400.f);
        const CollisionInfo collision = physics.step(actor, {&ceiling}, 0.05f);
        passed &= check(collision.ceilHit &&
                            nearlyEqual(actor.getPosition().y, 32.f),
                        "ceiling collision resolves on Y axis");
        passed &= check(ceiling.lastCollisionSide == 0,
                        "headbutt dispatches Person 4 bottom-side reaction");
    }

    {
        LevelManager level;
        auto cannon = std::make_unique<Cannon>(
            sf::Vector2f{32.f, 32.f},
            [&level](std::unique_ptr<GameObject> spawned) {
                level.addEntity(std::move(spawned));
            },
            0.1f, 1.f);
        level.addEntity(std::move(cannon), false, true);
        level.update(0.11f);
        passed &= check(level.getEntities().size() == 2,
                        "Cannon queues one uniquely-owned Bullet");

        Bullet* bullet = nullptr;
        for (const auto& entity : level.getEntities()) {
            if (auto* candidate = dynamic_cast<Bullet*>(entity.get())) {
                bullet = candidate;
            }
        }
        passed &= check(bullet != nullptr, "queued Bullet is registered once");
        if (bullet) bullet->deactivate();
        level.update(0.01f);
        passed &= check(level.getEntities().size() == 1,
                        "inactive Bullet is removed once after update");
    }

    {
        LevelManager level;
        auto target = std::make_shared<std::optional<sf::Vector2f>>(
            sf::Vector2f{128.f, 32.f});
        auto rocket = std::make_unique<Rocket>(
            sf::Vector2f{0.f, 32.f},
            [target]() -> std::optional<sf::Vector2f> { return *target; });
        Rocket* rocketView = rocket.get();
        level.addEntity(std::move(rocket));
        level.update(0.05f);
        passed &= check(rocketView->isActive() &&
                            rocketView->getPosition().x > 0.f,
                        "Rocket resolves a live target position");
        target->reset();
        level.update(0.05f);
        passed &= check(level.getEntities().empty(),
                        "Rocket deactivates and is removed when target disappears");
    }

    {
        LevelManager level;
        auto enemy = std::make_unique<TestEnemy>(sf::Vector2f{0.f, 0.f});
        TestEnemy* enemyView = enemy.get();
        level.addEntity(std::move(enemy), true);
        enemyView->setDead(true);
        level.update(0.01f);
        passed &= check(level.getEntities().empty() &&
                            level.getEnemies().empty() &&
                            level.getPhysicsEnemies().empty(),
                        "dead enemy removal rebuilds all non-owning views");
    }

    {
        PhysicsEngine physics(980.f);
        Mario player;
        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);

        std::vector<std::unique_ptr<GameObject>> fragments;
        std::unordered_set<GameObject*> delivered;
        int callbackCalls = 0;
        Brick brick([&](GameObject* spawned) {
            ++callbackCalls;
            if (spawned) {
                delivered.insert(spawned);
                fragments.emplace_back(spawned);
            }
        });
        brick.setPosition({64.f, 64.f});
        brick.setSize({32.f, 32.f});

        const CollisionInfo collision = physics.step(player, {&brick}, 0.05f);
        passed &= check(collision.ceilHit,
                        "real player headbutt reaches normal Brick bottom");
        passed &= check(!brick.isExist(),
                        "normal Brick reports inactive after headbutt");

        bool allFragments = fragments.size() == 4;
        for (const auto& fragment : fragments) {
            allFragments = allFragments &&
                dynamic_cast<BrickFragment*>(fragment.get()) != nullptr;
        }
        passed &= check(callbackCalls == 4 && delivered.size() == 4 &&
                            allFragments,
                        "normal Brick spawns exactly four unique BrickFragments");

        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);
        physics.step(player, {&brick}, 0.05f);
        passed &= check(callbackCalls == 4 && fragments.size() == 4,
                        "repeated normal Brick headbutt spawns no extra fragments");
    }

    {
        LevelManager level;
        Mario player;
        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);
        level.setPlayers({&player});

        int callbackCalls = 0;
        std::unordered_set<GameObject*> delivered;
        auto brick = std::make_unique<Brick>([&](GameObject* spawned) {
            ++callbackCalls;
            if (spawned) delivered.insert(spawned);
            level.addEntity(std::unique_ptr<GameObject>(spawned));
        });
        brick->setPosition({64.f, 64.f});
        brick->setSize({32.f, 32.f});
        level.addEntity(std::move(brick), false, true);

        level.update(0.05f);

        std::unordered_set<const GameObject*> fragmentAddresses;
        bool ownsOnlyFragments = level.getEntities().size() == 4;
        for (const auto& entity : level.getEntities()) {
            ownsOnlyFragments = ownsOnlyFragments &&
                dynamic_cast<const BrickFragment*>(entity.get()) != nullptr;
            fragmentAddresses.insert(entity.get());
        }
        passed &= check(callbackCalls == 4 && delivered.size() == 4 &&
                            fragmentAddresses.size() == 4,
                        "LevelManager adopts four BrickFragments exactly once");
        passed &= check(ownsOnlyFragments,
                        "LevelManager removes inactive Brick ownership");
        passed &= check(level.getBlocks().empty(),
                        "inactive Brick disappears from block collision view");

        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);
        PhysicsEngine physics(980.f);
        const CollisionInfo collision =
            physics.step(player, level.getBlocks(), 0.05f);
        passed &= check(!collision.ceilHit && player.getPosition().y < 98.f,
                        "subsequent physics no longer collides with removed Brick");
    }

    return passed ? 0 : 1;
}
