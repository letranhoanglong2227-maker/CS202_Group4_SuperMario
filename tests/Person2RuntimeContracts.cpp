#include "Entities/Base/Enemy.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Entities/Players/Mario.hpp"
#include "Levels/Managers/LevelManager.hpp"
#include "Levels/Stages/W3_LV3.hpp"
#include "Objects/Blocks/Block.hpp"
#include "Objects/Blocks/Brick.hpp"
#include "Objects/Blocks/BrickFragment.hpp"
#include "Objects/Blocks/MovingBlock.hpp"
#include "Objects/Environment/Bullet.hpp"
#include "Objects/Environment/Cannon.hpp"
#include "Objects/Environment/Lava.hpp"
#include "Objects/Environment/Pipe.hpp"
#include "Objects/Environment/Rocket.hpp"
#include "Objects/Environment/Trampoline.hpp"
#include "Objects/Environment/WinFlag.hpp"
#include "Physics/PhysicsEngine.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
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

class TestStageLevel final : public LevelManager {
public:
    using LevelManager::addStageEnemy;
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

    passed &= check(nearlyEqual(MapFormat::TILE_SIZE, 64.f),
                    "Group4 and Group5 share a 64px cell size");

    {
        TestStageLevel level;
        Enemy* patrol = level.addStageEnemy(
            "Goomba", {19.f, 0.f}, 10.f, 20.f);
        Enemy* defaultPatrol = level.addStageEnemy(
            "Goomba", {13415.f, 0.f});
        level.update(1.f);
        passed &= check(patrol && nearlyEqual(patrol->getPosition().x, 20.f) &&
                            !patrol->isFacingRight() && defaultPatrol &&
                            nearlyEqual(defaultPatrol->getPosition().x,
                                        13416.f) &&
                            !defaultPatrol->isFacingRight(),
                        "ported patrols preserve explicit and default Group5 bounds");
    }

    {
        MovingBlock block({0.f, 0.f}, 1, 10.f, 10.f);
        block.update(1.f);
        const bool movedRight = nearlyEqual(block.getPosition().x, 10.f) &&
                                nearlyEqual(block.getPosition().y, 0.f);
        block.update(1.f);
        const bool movedDownWithoutCornerPause =
            nearlyEqual(block.getPosition().x, 10.f) &&
            nearlyEqual(block.getPosition().y, 10.f);
        block.update(2.f);
        passed &= check(movedRight && movedDownWithoutCornerPause &&
                            nearlyEqual(block.getPosition().x, 0.f) &&
                            nearlyEqual(block.getPosition().y, 0.f) &&
                            nearlyEqual(block.getFrameDelta().x, -10.f) &&
                            nearlyEqual(block.getFrameDelta().y, -10.f),
                        "ported MovingBlock consumes full dt around the Group5 square path");

        block.update(4.f);
        passed &= check(
                            nearlyEqual(block.getPosition().x, 0.f) &&
                            nearlyEqual(block.getPosition().y, 0.f) &&
                            nearlyEqual(block.getFrameDelta().x, 0.f) &&
                            nearlyEqual(block.getFrameDelta().y, 0.f),
                        "MovingBlock square path remains stable after a full-loop dt");
    }

    {
        Pipe pipe({10.f, 20.f}, 3);
        const sf::FloatRect collider = pipe.hitbox.getGlobalBounds();
        Pipe clamped({}, 0);
        LevelManager owner;
        auto ownedPipe = std::make_unique<Pipe>(sf::Vector2f{}, 2);
        Pipe* pipeView = ownedPipe.get();
        owner.addEntity(std::move(ownedPipe), false, true);
        passed &= check(
            nearlyEqual(pipe.getSize().x, 2.f * MapFormat::TILE_SIZE) &&
                nearlyEqual(pipe.getSize().y, 3.f * MapFormat::TILE_SIZE) &&
                collider.position == sf::Vector2f(10.f, 20.f) &&
                collider.size == pipe.getSize() &&
                nearlyEqual(clamped.getSize().y, MapFormat::TILE_SIZE) &&
                owner.getBlocks().size() == 1 &&
                owner.getBlocks().front() == pipeView,
            "Pipe collider preserves two-tile width and clamped height");

        MovingBlock horizontal({}, 1, {10.f, 0.f}, 20.f);
        horizontal.update(0.5f);
        const bool reachedHorizontalEnd =
            nearlyEqual(horizontal.getPosition().x, 10.f) &&
            nearlyEqual(horizontal.getFrameDelta().x, 10.f);
        horizontal.update(0.5f);

        MovingBlock vertical({}, 1, {0.f, -10.f}, 20.f);
        vertical.update(0.5f);
        const bool reachedVerticalEnd =
            nearlyEqual(vertical.getPosition().y, -10.f) &&
            nearlyEqual(vertical.getFrameDelta().y, -10.f);
        vertical.update(0.5f);
        passed &= check(reachedHorizontalEnd && reachedVerticalEnd &&
                            nearlyEqual(horizontal.getPosition().x, 0.f) &&
                            nearlyEqual(vertical.getPosition().y, 0.f),
                        "MovingBlock clamps horizontal and vertical endpoints");
    }

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
        TestEnemy actor({-1000.f, 700.f});
        MovementComponent* movement = actor.getMovementComponent();
        const sf::FloatRect world({64.f, 0.f}, {640.f, 600.f});
        const float rightEdge = world.position.x + world.size.x -
                                actor.hitbox.getGlobalBounds().size.x;

        movement->setVelocity(-400.f, 123.f);
        physics.enforceHorizontalBounds(actor, world);
        const bool leftOverflowResolved =
            nearlyEqual(actor.getPosition().x, world.position.x) &&
            nearlyEqual(actor.getPosition().y, 700.f) &&
            nearlyEqual(movement->getVelocity().x, 0.f) &&
            nearlyEqual(movement->getVelocity().y, 123.f);

        actor.setPosition({world.position.x + world.size.x + 1000.f, 700.f});
        movement->setVelocity(400.f, 123.f);
        physics.enforceHorizontalBounds(actor, world);
        const bool rightOverflowResolved =
            nearlyEqual(actor.getPosition().x, rightEdge) &&
            nearlyEqual(actor.getPosition().y, 700.f) &&
            nearlyEqual(movement->getVelocity().x, 0.f) &&
            nearlyEqual(movement->getVelocity().y, 123.f);

        actor.setPosition({world.position.x, 700.f});
        movement->setVelocity(-50.f, 123.f);
        physics.enforceHorizontalBounds(actor, world);
        const bool exactEdgeStopsOutwardMotion =
            nearlyEqual(movement->getVelocity().x, 0.f);

        movement->setVelocity(50.f, 123.f);
        physics.enforceHorizontalBounds(actor, world);
        const bool leftEdgeKeepsInwardMotion =
            nearlyEqual(movement->getVelocity().x, 50.f);

        actor.setPosition({rightEdge, 700.f});
        movement->setVelocity(-50.f, 123.f);
        physics.enforceHorizontalBounds(actor, world);
        const bool rightEdgeKeepsInwardMotion =
            nearlyEqual(movement->getVelocity().x, -50.f);

        passed &= check(leftOverflowResolved && rightOverflowResolved &&
                            exactEdgeStopsOutwardMotion &&
                            leftEdgeKeepsInwardMotion &&
                            rightEdgeKeepsInwardMotion,
                        "horizontal world bounds resolve both edges without clamping Y");

        const sf::FloatRect smallerThanActor(
            {25.f, 0.f}, {16.f, 600.f});
        actor.setPosition({100.f, 700.f});
        movement->setVelocity(-50.f, 123.f);
        physics.enforceHorizontalBounds(actor, smallerThanActor);
        passed &= check(nearlyEqual(actor.getPosition().x, 25.f) &&
                            nearlyEqual(movement->getVelocity().x, 0.f),
                        "horizontal bounds handle worlds narrower than the hitbox");
    }

    {
        const sf::FloatRect target({64.f, 64.f}, {64.f, 64.f});
        const auto classify = [&target](sf::Vector2f previous,
                                        sf::Vector2f current) {
            return PhysicsEngine::classifyAabbContact(
                sf::FloatRect(previous, {32.f, 32.f}),
                sf::FloatRect(current, {32.f, 32.f}), target);
        };

        const bool sidesAreStable =
            classify({64.f, 20.f}, {64.f, 40.f}) ==
                AabbContactSide::Bottom &&
            classify({64.f, 140.f}, {64.f, 112.f}) ==
                AabbContactSide::Top &&
            classify({20.f, 64.f}, {40.f, 64.f}) ==
                AabbContactSide::Right &&
            classify({140.f, 64.f}, {112.f, 64.f}) ==
                AabbContactSide::Left &&
            classify({0.f, 0.f}, {1.f, 1.f}) == AabbContactSide::None;

        // Both axes enter at the same time. Horizontal is the safe fallback:
        // an ambiguous corner must not be reported as a stomp.
        const bool cornerIsNotFalseStomp =
            classify({16.f, 16.f}, {40.f, 40.f}) ==
            AabbContactSide::Right;
        passed &= check(sidesAreStable && cornerIsNotFalseStomp,
                        "AABB contact classification uses motion and avoids false corner stomps");
    }

    {
        PhysicsEngine physics(0.f);
        const sf::FloatRect smallPlayer({64.f, 64.f}, {64.f, 64.f});
        const sf::Vector2f poweredSize{64.f, 128.f};
        const std::vector<sf::FloatRect> clearBlocks{
            sf::FloatRect({256.f, 0.f}, {64.f, 64.f})};
        const std::vector<sf::FloatRect> lowCeiling{
            sf::FloatRect({64.f, 0.f}, {64.f, 32.f})};
        passed &= check(physics.canGrow(smallPlayer, poweredSize, clearBlocks) &&
                            !physics.canGrow(smallPlayer, poweredSize, lowCeiling),
                        "safe growth preserves feet and rejects a low ceiling");
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
        PhysicsEngine physics(0.f);
        Trampoline trampoline({0.f, 64.f}, -500.f);
        TestEnemy first({0.f, 20.f});
        TestEnemy second({0.f, 20.f});
        first.getMovementComponent()->setVelocity(0.f, 300.f);
        second.getMovementComponent()->setVelocity(0.f, 300.f);
        const CollisionInfo firstHit = physics.step(first, {&trampoline}, 0.05f);
        const CollisionInfo secondHit = physics.step(second, {&trampoline}, 0.05f);
        passed &= check(firstHit.collided && secondHit.collided &&
                            nearlyEqual(first.getMovementComponent()
                                            ->getVelocity().y,
                                        -500.f) &&
                            nearlyEqual(second.getMovementComponent()
                                            ->getVelocity().y,
                                        -500.f),
                        "Trampoline launches both players without shared debounce");

        MovingBlock platform({0.f, 64.f}, 1, sf::Vector2f{10.f, 0.f}, 10.f);
        platform.update(1.f);
        TestEnemy rider({10.f, 20.f});
        rider.getMovementComponent()->setVelocity(0.f, 300.f);
        const CollisionInfo ride = physics.step(rider, {&platform}, 0.05f);
        passed &= check(ride.grounded &&
                            nearlyEqual(platform.getFrameDelta().x, 10.f) &&
                            nearlyEqual(rider.getPosition().x, 20.f),
                        "MovingBlock frame delta carries a landing actor");
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
        passed &= check(bullet && nearlyEqual(bullet->getPosition().x, 96.f) &&
                            nearlyEqual(bullet->getPosition().y, 48.f),
                        "queued Bullet is registered once without a birth-frame update");
        if (bullet) bullet->deactivate();
        level.update(0.01f);
        passed &= check(level.getEntities().size() == 1,
                        "inactive Bullet is removed once after update");
    }

    {
        LevelManager level;
        Mario smallPlayer;
        smallPlayer.setPosition({0.f, 0.f});
        level.setPlayers({&smallPlayer});
        level.addEntity(std::make_unique<Lava>(sf::Vector2f{0.f, 0.f}));
        int deathCalls = 0;
        level.setPlayerDeathCallback(
            [&](PlayerManager&) { ++deathCalls; });
        level.update(0.01f);
        level.update(0.01f);
        passed &= check(smallPlayer.isDead() && deathCalls == 1,
                        "Lava reports a small-player death exactly once");
    }

    {
        LevelManager level;
        Mario poweredPlayer;
        poweredPlayer.setBig(true);
        poweredPlayer.setPosition({0.f, 0.f});
        level.setPlayers({&poweredPlayer});
        level.addEntity(std::make_unique<Lava>(sf::Vector2f{0.f, 0.f}));
        int deathCalls = 0;
        level.setPlayerDeathCallback(
            [&](PlayerManager&) { ++deathCalls; });
        level.update(0.01f);
        passed &= check(!poweredPlayer.isDead() && !poweredPlayer.isBig() &&
                            deathCalls == 0,
                        "Lava preserves powered-player downgrade semantics");
    }

    {
        LevelManager level;
        Mario firePlayer;
        firePlayer.setFire(true);
        firePlayer.setPosition({0.f, 0.f});
        level.setPlayers({&firePlayer});
        level.addEntity(std::make_unique<Lava>(sf::Vector2f{0.f, 0.f}));
        int deathCalls = 0;
        level.setPlayerDeathCallback(
            [&](PlayerManager&) { ++deathCalls; });
        level.update(0.01f);
        passed &= check(!firePlayer.isDead() && !firePlayer.isFire() &&
                            firePlayer.isBig() && deathCalls == 0,
                        "Lava preserves fire-player downgrade semantics");
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
        auto block = std::make_unique<TestBlock>(
            sf::Vector2f{64.f, 64.f}, sf::Vector2f{64.f, 64.f});
        level.addEntity(std::move(block), false, true);
        level.addEntity(std::make_unique<Bullet>(
            sf::Vector2f{64.f, 64.f}, sf::Vector2f{}));
        level.update(0.01f);
        const bool bulletRemoved = std::none_of(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<Bullet*>(entity.get()) != nullptr;
            });
        passed &= check(bulletRemoved,
                        "Bullet deactivates and is removed on a Block hit");

        auto target = std::make_shared<std::optional<sf::Vector2f>>(
            sf::Vector2f{96.f, 96.f});
        level.spawnRocket(
            {64.f, 64.f},
            [target]() -> std::optional<sf::Vector2f> { return *target; });
        level.update(0.01f);
        const bool rocketRemoved = std::none_of(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<Rocket*>(entity.get()) != nullptr;
            });
        passed &= check(rocketRemoved,
                        "production Rocket spawn is owned and removed on Block hit");
    }

    {
        LevelManager level;
        level.addEntity(std::make_unique<TestBlock>(
                            sf::Vector2f{96.f, 64.f},
                            sf::Vector2f{32.f, 64.f}),
                        false, true);
        level.addEntity(std::make_unique<Bullet>(
            sf::Vector2f{0.f, 64.f}, sf::Vector2f{2000.f, 0.f}, 1.f));
        level.spawnRocket({0.f, 80.f}, sf::Vector2f{2000.f, 0.f}, 1.f);
        level.update(0.1f);

        const bool fastProjectilesRemoved = std::none_of(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<Bullet*>(entity.get()) != nullptr ||
                       dynamic_cast<Rocket*>(entity.get()) != nullptr;
            });
        passed &= check(fastProjectilesRemoved,
                        "swept AABB removes fast Bullet and Rocket crossing a Block");
    }

    {
        LevelManager level;
        level.spawnRocket({0.f, 0.f}, sf::Vector2f{40.f, 0.f}, 1.f);
        Rocket* rocket = level.getEntities().empty()
                             ? nullptr
                             : dynamic_cast<Rocket*>(
                                   level.getEntities().front().get());
        level.update(0.25f);
        passed &= check(rocket && rocket->isActive() &&
                            nearlyEqual(rocket->getPosition().x, 10.f),
                        "straight Rocket request has a production-owned path");
    }

    {
        LevelManager level;
        Mario first;
        Mario second;
        first.setPosition({0.f, 0.f});
        second.setPosition({0.f, 0.f});
        level.setPlayers({&first, &second});
        level.addEntity(std::make_unique<Bullet>(
            sf::Vector2f{0.f, 0.f}, sf::Vector2f{}, 1.f));
        int deathCalls = 0;
        PlayerManager* affected = nullptr;
        level.setPlayerDeathCallback([&](PlayerManager& player) {
            ++deathCalls;
            affected = &player;
        });
        level.update(0.01f);
        level.update(0.01f);
        passed &= check(first.isDead() && !second.isDead() &&
                            deathCalls == 1 && affected == &first,
                        "one Bullet damages one overlapping player exactly once");
    }

    {
        int completionCalls = 0;
        const sf::Vector2f baseAnchor{128.f, 512.f};
        WinFlag flag(baseAnchor, [&] { ++completionCalls; });
        const sf::Vector2f expectedTop{
            baseAnchor.x,
            baseAnchor.y - 4.f * MapFormat::TILE_SIZE};
        flag.activate();
        flag.activate();
        flag.update(0.5f);
        const bool callbackWaited = completionCalls == 0;
        flag.update(0.5f);
        flag.update(1.f);
        passed &= check(flag.getPosition() == baseAnchor &&
                            flag.hitbox.getPosition() == expectedTop &&
                            callbackWaited && completionCalls == 1,
                        "WinFlag anchors upward and completes after one slide");
    }

    {
        ConfiguredLevel level(1, 1);
        WinFlag* flag = nullptr;
        for (const auto& entity : level.getEntities()) {
            if (auto* candidate = dynamic_cast<WinFlag*>(entity.get())) {
                flag = candidate;
                break;
            }
        }
        int completionCalls = 0;
        level.setLevelCompletedCallback([&] { ++completionCalls; });
        if (flag) flag->activate();
        level.update(0.5f);
        const bool callbackWaited = completionCalls == 0;
        level.update(0.5f);
        level.update(0.1f);
        passed &= check(level.isLoaded() && flag && callbackWaited &&
                            completionCalls == 1,
                        "LevelManager defers WinFlag completion until animation ends");
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
        Mario sameFrameFollower;
        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);
        player.setGrounded(false);
        sameFrameFollower.setPosition({64.f, 98.f});
        sameFrameFollower.getMovementComponent()->setVelocity(0.f, -200.f);
        sameFrameFollower.setGrounded(false);
        level.setPlayers({&player, &sameFrameFollower});

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

        passed &= check(callbackCalls == 4 &&
                            nearlyEqual(player.getPosition().y, 96.f) &&
                            nearlyEqual(
                                player.getMovementComponent()->getVelocity().y,
                                0.f),
                        "first actor breaks normal Brick before same-frame follower");
        passed &= check(sameFrameFollower.getPosition().y < 96.f &&
                            sameFrameFollower.getMovementComponent()
                                    ->getVelocity().y < 0.f,
                        "later same-frame actor ignores retained inactive Brick");

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

    {
        BrickFragment fragment({0.f, 1079.f}, {0.f, 0.f});
        fragment.update(0.05f);
        passed &= check(fragment.getPosition().y > 1080.f &&
                            !fragment.isExist(),
                        "real BrickFragment expires through its lifecycle rule");
    }

    {
        LevelManager level;
        auto fragment = std::make_unique<BrickFragment>(
            sf::Vector2f{0.f, 1079.f}, sf::Vector2f{0.f, 0.f});
        BrickFragment* fragmentView = fragment.get();
        level.addEntity(std::move(fragment), false, true);
        const bool registeredBeforeExpiry =
            level.getEntities().size() == 1 && level.getBlocks().size() == 1 &&
            level.getBlocks().front() == fragmentView;

        level.update(0.05f);

        passed &= check(registeredBeforeExpiry && level.getEntities().empty() &&
                            level.getBlocks().empty(),
                        "expired BrickFragment leaves no owned or block-view pointer");
    }

    {
        struct MapExpectation {
            const char* name;
            unsigned int width;
            std::size_t spawnCount;
        };
        constexpr std::array<MapExpectation, 9> maps{{
            {"W1_LV1", 211, 602}, {"W1_LV2", 200, 606},
            {"W1_LV3", 200, 543}, {"W2_LV1", 200, 533},
            {"W2_LV2", 200, 401}, {"W2_LV3", 200, 901},
            {"W3_LV1", 200, 815}, {"W3_LV2", 200, 872},
            {"W3_LV3", 25, 115},
        }};
        const std::string assetPrefix =
            std::filesystem::exists("assets/textures")
                ? "assets/textures/"
                : "../assets/textures/";

        for (const MapExpectation& expected : maps) {
            MapManager map;
            const std::string path = assetPrefix + "LevelSketch_" +
                                     std::string(expected.name) + ".png";
            const bool loaded = map.loadMap(path);
            passed &= check(loaded && map.getDiagnostics().empty() &&
                                map.getMapWidth() == expected.width &&
                                map.getSpawns().size() == expected.spawnCount,
                            std::string("Group5 map ") + expected.name +
                                " parses through the SFML 3.1 loader");
            if (loaded && std::string_view(expected.name) == "W3_LV3") {
                const auto playerSpawn = std::find_if(
                    map.getSpawns().begin(), map.getSpawns().end(),
                    [](const MapSpawnInfo& spawn) {
                        return spawn.type == MapObjectType::Player1Spawn;
                    });
                passed &= check(
                    playerSpawn != map.getSpawns().end() &&
                        nearlyEqual(playerSpawn->position.x, 768.f) &&
                        nearlyEqual(playerSpawn->position.y, 768.f),
                    "Group5 sketch pixels map to unchanged 64px coordinates");
            }
        }

        W3_LV3 level;
        const std::size_t mapEnemyCount = static_cast<std::size_t>(
            std::count_if(level.getMapManager().getSpawns().begin(),
                          level.getMapManager().getSpawns().end(),
                          [](const MapSpawnInfo& spawn) {
                              return spawn.type == MapObjectType::Goomba ||
                                     spawn.type == MapObjectType::Koopa ||
                                     spawn.type == MapObjectType::FlyingKoopa ||
                                     spawn.type == MapObjectType::Heriss ||
                                     spawn.type == MapObjectType::PeteyPiranha ||
                                     spawn.type == MapObjectType::Bowser;
                          }));
        const auto bowser = std::find_if(
            level.getEnemies().begin(), level.getEnemies().end(),
            [](const Enemy* enemy) {
                return dynamic_cast<const Bowser*>(enemy) != nullptr;
            });
        passed &= check(level.isLoaded() && level.getWorld() == 3 &&
                            level.getLevel() == 3 &&
                            level.getMapManager().getMapWidth() == 25 &&
                            level.getEnemies().size() == mapEnemyCount + 1 &&
                            bowser != level.getEnemies().end() &&
                            nearlyEqual((*bowser)->getSize().x, 128.f) &&
                            nearlyEqual((*bowser)->getSize().y, 140.f),
                        "W3_LV3 adds the Group5 Bowser on the 64px map");
    }

    {
        Mario widePlayer;
        Mario narrowPlayer;
        ConfiguredLevel wideLevel(1, 1, {&widePlayer});
        ConfiguredLevel narrowLevel(3, 3, {&narrowPlayer});
        const auto wideBounds = wideLevel.getWorldBounds();
        const auto narrowBounds = narrowLevel.getWorldBounds();

        if (wideBounds) {
            widePlayer.setPosition(
                {wideBounds->position.x + wideBounds->size.x -
                     widePlayer.hitbox.getGlobalBounds().size.x - 1.f,
                 -256.f});
        }
        if (narrowBounds) {
            narrowPlayer.setPosition(
                {narrowBounds->position.x + narrowBounds->size.x -
                     narrowPlayer.hitbox.getGlobalBounds().size.x - 1.f,
                 -256.f});
        }
        widePlayer.getMovementComponent()->setVelocity(200.f, 0.f);
        narrowPlayer.getMovementComponent()->setVelocity(200.f, 0.f);
        wideLevel.update(0.05f);
        narrowLevel.update(0.05f);

        const auto atRightEdge = [](const Mario& player,
                                    const sf::FloatRect& bounds) {
            const sf::FloatRect playerBounds = player.hitbox.getGlobalBounds();
            return nearlyEqual(playerBounds.position.x,
                               bounds.position.x + bounds.size.x -
                                   playerBounds.size.x) &&
                   nearlyEqual(player.getMovementComponent()->getVelocity().x,
                               0.f);
        };
        passed &= check(wideBounds && narrowBounds &&
                            nearlyEqual(wideBounds->size.x,
                                        211.f * MapFormat::TILE_SIZE) &&
                            nearlyEqual(narrowBounds->size.x,
                                        25.f * MapFormat::TILE_SIZE) &&
                            atRightEdge(widePlayer, *wideBounds) &&
                            atRightEdge(narrowPlayer, *narrowBounds),
                        "loaded levels clamp same-frame overshoot at their dynamic widths");
    }

    {
        Mario player;
        W3_LV3 level({&player});
        const auto bounds = level.getWorldBounds();
        passed &= check(bounds && nearlyEqual(bounds->size.x, 25.f * 64.f) &&
                            nearlyEqual(bounds->size.y, 15.f * 64.f),
                        "loaded level exposes map-derived world bounds");

        const sf::Vector2f worldSize = bounds ? bounds->size : sf::Vector2f{};
        level.addEntity(std::make_unique<Bullet>(
            sf::Vector2f{worldSize.x + 2.f * MapFormat::TILE_SIZE, 64.f},
            sf::Vector2f{}));
        level.spawnRocket(
            {worldSize.x + 2.f * MapFormat::TILE_SIZE, 128.f},
            sf::Vector2f{}, 1.f);
        level.update(0.01f);
        const bool offWorldProjectilesRemoved = std::none_of(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<Bullet*>(entity.get()) != nullptr ||
                       dynamic_cast<Rocket*>(entity.get()) != nullptr;
            });
        passed &= check(offWorldProjectilesRemoved,
                        "loaded level culls off-world Bullet and Rocket");

        int deathCalls = 0;
        PlayerManager* deadPlayer = nullptr;
        level.setPlayerDeathCallback([&](PlayerManager& affected) {
            ++deathCalls;
            deadPlayer = &affected;
        });
        player.setPosition({64.f, worldSize.y + MapFormat::TILE_SIZE + 1.f});
        level.update(0.01f);
        level.update(0.01f);
        passed &= check(player.isDead() && deathCalls == 1 &&
                            deadPlayer == &player,
                        "pit death is fatal and reported exactly once");
    }

    return passed ? 0 : 1;
}
