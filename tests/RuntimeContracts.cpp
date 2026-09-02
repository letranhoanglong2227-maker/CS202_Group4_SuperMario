#include "Entities/Base/Enemy.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Entities/Enemies/Goomba.hpp"
#include "Entities/Enemies/Heriss.hpp"
#include "Entities/Enemies/Koopa.hpp"
#include "Entities/Enemies/PeteyPiranha.hpp"
#include "Entities/Players/Luigi.hpp"
#include "Entities/Players/Mario.hpp"
#include "Levels/Managers/LevelManager.hpp"
#include "Levels/LevelFactory.hpp"
#include "Levels/Stages/W3_LV3.hpp"
#include "Objects/Blocks/Block.hpp"
#include "Objects/Blocks/Brick.hpp"
#include "Objects/Blocks/BrickFragment.hpp"
#include "Objects/Blocks/CloudBlock.hpp"
#include "Objects/Blocks/CoinBlock.hpp"
#include "Objects/Blocks/MushroomBlock.hpp"
#include "Objects/Blocks/MovingBlock.hpp"
#include "Objects/Environment/Bullet.hpp"
#include "Objects/Environment/Cannon.hpp"
#include "Objects/Environment/Lava.hpp"
#include "Objects/Environment/Pipe.hpp"
#include "Objects/Environment/Rocket.hpp"
#include "Objects/Environment/Trampoline.hpp"
#include "Objects/Environment/WinFlag.hpp"
#include "Objects/Items/Coin.hpp"
#include "Objects/Items/Fireball.hpp"
#include "Objects/Items/Mushroom.hpp"
#include "Objects/Items/ProjectileSpawnRequest.hpp"
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
        LevelManager level;
        Mario fallingPlayer;
        fallingPlayer.setPosition({0.f, 0.f});
        fallingPlayer.setGrounded(false);
        level.setPlayers({&fallingPlayer});
        level.update(0.1f);
        passed &= check(nearlyEqual(
                            fallingPlayer.getMovementComponent()->getVelocity().y,
                            250.f),
                        "LevelManager uses the shared 2500px/s2 gameplay gravity");
    }

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
        CoinBlock coinBlock(1);
        coinBlock.setPosition({100.f, 200.f});
        coinBlock.setSizeBlock({64.f, 64.f});
        coinBlock.update(0.f);
        coinBlock.reactToCollision(COLLISION_BOTTOM);
        coinBlock.update(0.1f);
        const bool coinAtPeak = nearlyEqual(coinBlock.getPosition().y, 190.f) &&
                                nearlyEqual(coinBlock.hitbox.getPosition().y, 200.f);
        coinBlock.update(0.1f);

        MushroomBlock mushroomBlock;
        mushroomBlock.setPosition({100.f, 200.f});
        mushroomBlock.setSizeBlock({64.f, 64.f});
        mushroomBlock.update(0.f);
        mushroomBlock.reactToCollision(COLLISION_BOTTOM);
        mushroomBlock.update(0.1f);
        const bool mushroomAtPeak =
            nearlyEqual(mushroomBlock.getPosition().y, 190.f) &&
            nearlyEqual(mushroomBlock.hitbox.getPosition().y, 200.f);
        mushroomBlock.update(0.1f);

        passed &= check(
            coinAtPeak && mushroomAtPeak &&
                nearlyEqual(coinBlock.getPosition().y, 200.f) &&
                nearlyEqual(mushroomBlock.getPosition().y, 200.f),
            "reward blocks bounce 10px once without moving their colliders");
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
        passed &= check(ceiling.lastCollisionSide == -1,
                        "enemy ceiling contact does not activate a Block");
    }

    {
        PhysicsEngine physics(2500.f);
        TestEnemy actor({0.f, 0.f});
        TestBlock floor({0.f, 96.f}, {64.f, 64.f});
        actor.getMovementComponent()->setVelocity(0.f, 2000.f);
        const CollisionInfo collision = physics.step(actor, {&floor}, 0.1f);
        passed &= check(collision.grounded &&
                            nearlyEqual(actor.getPosition().y, 64.f),
                        "physics substeps prevent a slow-frame floor tunnel");
    }

    {
        PhysicsEngine physics(2500.f);
        TestEnemy actor({0.f, 0.f});
        actor.getMovementComponent()->setVelocity(0.f, 1390.f);
        physics.step(actor, {}, 0.1f);
        passed &= check(nearlyEqual(
                            actor.getMovementComponent()->getVelocity().y,
                            1400.f),
                        "physics enforces the shared terminal fall speed");
    }

    {
        PhysicsEngine physics(0.f);
        Trampoline referenceTrampoline;
        Trampoline trampoline({0.f, 64.f}, -500.f);
        TestEnemy first({0.f, 20.f});
        TestEnemy second({0.f, 20.f});
        first.getMovementComponent()->setVelocity(0.f, 300.f);
        second.getMovementComponent()->setVelocity(0.f, 300.f);
        const CollisionInfo firstHit = physics.step(first, {&trampoline}, 0.05f);
        const CollisionInfo secondHit = physics.step(second, {&trampoline}, 0.05f);
        passed &= check(nearlyEqual(referenceTrampoline.getLaunchVelocity(),
                                    -1700.f) &&
                            firstHit.collided && secondHit.collided &&
                            nearlyEqual(first.getMovementComponent()
                                            ->getVelocity().y,
                                        -500.f) &&
                            nearlyEqual(second.getMovementComponent()
                                            ->getVelocity().y,
                                        -500.f),
                        "Trampoline uses Group5 force and launches both actors without shared debounce");

        MovingBlock platform({0.f, 64.f}, 1, sf::Vector2f{10.f, 0.f}, 10.f);
        platform.update(1.f);
        TestEnemy rider({10.f, 20.f});
        rider.getMovementComponent()->setVelocity(0.f, 300.f);
        const CollisionInfo ride = physics.step(rider, {&platform}, 0.05f);
        passed &= check(ride.grounded &&
                            nearlyEqual(platform.getFrameDelta().x, 10.f) &&
                            nearlyEqual(rider.getPosition().x, 20.f),
                        "MovingBlock frame delta carries a landing actor");

        MovingBlock descending(
            {0.f, 100.f}, 1, 20.f, 100.f, "basic_ground_mid");
        descending.update(0.2f); // Reach the right edge.
        TestEnemy descendingRider({20.f, 68.f});
        descending.update(0.1f); // Move down by 10px.
        descendingRider.getMovementComponent()->setVelocity(0.f, 300.f);
        const CollisionInfo descendingRide =
            physics.step(descendingRider, {&descending}, 0.05f);
        passed &= check(
            descendingRide.grounded &&
                nearlyEqual(descendingRider.hitbox.getGlobalBounds().position.y +
                                descendingRider.hitbox.getGlobalBounds().size.y,
                            descending.hitbox.getGlobalBounds().position.y),
            "descending MovingBlock snaps the rider once without embedding it");

        PhysicsEngine gameplayPhysics(2500.f);
        MovingBlock smoothDescent(
            {0.f, 100.f}, 1, 20.f, 100.f, "basic_ground_mid");
        smoothDescent.update(0.2f); // Reach the top-right corner.
        Mario standingRider;
        standingRider.setPosition({20.f, 36.f});
        standingRider.setGrounded(true);
        bool continuouslyGrounded = true;
        for (int frame = 0; frame < 10; ++frame) {
            smoothDescent.update(0.016f);
            const CollisionInfo frameContact = gameplayPhysics.step(
                standingRider, {&smoothDescent}, 0.016f);
            const sf::FloatRect riderBounds =
                standingRider.hitbox.getGlobalBounds();
            continuouslyGrounded = continuouslyGrounded &&
                frameContact.grounded && standingRider.isGrounded() &&
                nearlyEqual(riderBounds.position.y + riderBounds.size.y,
                            smoothDescent.hitbox.getGlobalBounds().position.y);
        }
        passed &= check(
            continuouslyGrounded,
            "descending MovingBlock carries a standing player without grounded-state flicker");
    }

    {
        LevelManager level;
        auto block = std::make_unique<MushroomBlock>();
        block->setPosition({100.f, 200.f});
        block->setSizeBlock({64.f, 64.f});
        block->update(0.f);
        MushroomBlock* blockView = block.get();

        auto enemy = std::make_unique<TestEnemy>(sf::Vector2f{116.f, 168.f});
        level.addEntity(std::move(block), false, true);
        level.addEntity(std::move(enemy));
        blockView->reactToCollision(COLLISION_BOTTOM);
        level.update(0.016f);
        passed &= check(
            level.getEnemies().empty(),
            "a rising MushroomBlock defeats an enemy standing on top");
    }

    {
        LevelManager level;
        auto petey = std::make_unique<PeteyPiranha>(
            sf::Vector2f{100.f, 100.f});
        auto walker = std::make_unique<Goomba>(sf::Vector2f{100.f, 100.f});
        Enemy* peteyView = petey.get();
        level.addEntity(std::move(petey));
        level.addEntity(std::move(walker), true);
        const sf::Vector2f anchor = peteyView->getPosition();
        level.update(0.016f);
        passed &= check(peteyView->getPosition() == anchor,
                        "walking enemies cannot displace Group5-style fixed Petey");
    }

    {
        LevelManager level;
        auto cloud = std::make_unique<CloudBlock>(
            sf::Vector2f{0.f, 64.f}, 4, 0.f);
        CloudBlock* cloudView = cloud.get();
        level.addEntity(std::move(cloud), false, true);
        level.update(4.f);
        const bool hiddenButOwned = !cloudView->isExist() &&
            level.getEntities().size() == 1 && level.getBlocks().size() == 1;
        level.update(2.5f);
        passed &= check(hiddenButOwned && cloudView->isExist(),
                        "CloudBlock hides and reappears without losing ownership");
    }

    {
        LevelManager level;
        auto cannon = std::make_unique<Cannon>(
            sf::Vector2f{32.f, 32.f},
            [&level](std::unique_ptr<GameObject> spawned) {
                level.addEntity(std::move(spawned));
            },
            0.1f);
        level.addEntity(std::move(cannon), false, true);
        level.update(0.11f);
        passed &= check(level.getEntities().size() == 3,
                        "Cannon queues two uniquely-owned Rockets");

        std::vector<Rocket*> cannonRockets;
        for (const auto& entity : level.getEntities()) {
            if (auto* rocket = dynamic_cast<Rocket*>(entity.get()))
                cannonRockets.push_back(rocket);
        }
        std::sort(cannonRockets.begin(), cannonRockets.end(),
                  [](const Rocket* lhs, const Rocket* rhs) {
                      return lhs->getPosition().x < rhs->getPosition().x;
                  });
        passed &= check(cannonRockets.size() == 2 &&
                            nearlyEqual(cannonRockets[0]->getPosition().x, -32.f) &&
                            nearlyEqual(cannonRockets[1]->getPosition().x, 96.f) &&
                            nearlyEqual(cannonRockets[0]->getPosition().y, 31.f),
                        "queued Rockets start on both sides without a birth update");
        for (Rocket* rocket : cannonRockets) rocket->deactivate();
        level.update(0.01f);
        passed &= check(level.getEntities().size() == 1,
                        "inactive Cannon Rockets are removed once after update");
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
        passed &= check(!smallPlayer.isDead() &&
                            smallPlayer.getHealth() == 2 &&
                            smallPlayer.isImmortal() && deathCalls == 0,
                        "Lava matches Group5's one-damage contact without frame drain");
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
        first.setHealth(1);
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
        const sf::Vector2f initialFlagPosition{
            baseAnchor.x + 25.f, baseAnchor.y};
        const bool startsAtGroup5Base =
            flag.getFlagVisualPosition() == initialFlagPosition;
        flag.activate();
        flag.activate();
        flag.update(0.5f);
        const bool callbackWaited = completionCalls == 0;
        const bool movedHalfwayUp = nearlyEqual(
            flag.getFlagVisualPosition().y, baseAnchor.y - 235.f);
        flag.update(0.5f);
        const bool reachedGroup5Top = nearlyEqual(
            flag.getFlagVisualPosition().y, baseAnchor.y - 470.f);
        flag.update(1.f);
        passed &= check(flag.getPosition() == baseAnchor &&
                            flag.hitbox.getPosition() == baseAnchor &&
                            flag.hitbox.getSize() ==
                                sf::Vector2f(MapFormat::TILE_SIZE,
                                             MapFormat::TILE_SIZE) &&
                            startsAtGroup5Base && movedHalfwayUp &&
                            reachedGroup5Top && callbackWaited &&
                            completionCalls == 1,
                        "WinFlag preserves Group5 base collider and upward 470px slide");
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
        LevelManager level;
        Mario player;
        player.setHealth(1);
        player.setPosition({0.f, 0.f});
        player.getMovementComponent()->setVelocity(0.f, 100.f);
        level.setPlayers({&player});
        auto goomba = std::make_unique<Goomba>(sf::Vector2f{0.f, 64.f});
        Goomba* goombaView = goomba.get();
        level.addEntity(std::move(goomba));
        int score = 0;
        int deaths = 0;
        level.setScoreChangedCallback([&](int delta) { score += delta; });
        level.setPlayerDeathCallback([&](PlayerManager&) { ++deaths; });

        level.update(0.05f);
        passed &= check(goombaView->isStomped() && score == 300 &&
                            deaths == 0 &&
                            nearlyEqual(player.getMovementComponent()
                                            ->getVelocity().y,
                                        -500.f),
                        "Goomba stomp resolves once, scores, and bounces player");

        level.update(1.01f);
        passed &= check(level.getEnemies().empty() && score == 300,
                        "stomped Goomba expires once and leaves no enemy view");
    }

    {
        LevelManager level;
        Mario player;
        player.setHealth(1);
        player.setPosition({0.f, 0.f});
        player.getMovementComponent()->setVelocity(100.f, 0.f);
        level.setPlayers({&player});
        level.addEntity(std::make_unique<Goomba>(sf::Vector2f{64.f, 0.f}));
        int deaths = 0;
        level.setPlayerDeathCallback([&](PlayerManager&) { ++deaths; });
        level.update(0.05f);
        level.update(0.05f);
        passed &= check(player.isDead() && deaths == 1,
                        "Goomba side contact kills a small player exactly once");
    }

    {
        LevelManager level;
        Mario player;
        player.setPosition({0.f, 0.f});
        player.setImmortal(true);
        player.getMovementComponent()->setVelocity(100.f, 0.f);
        level.setPlayers({&player});
        level.addEntity(std::make_unique<Goomba>(sf::Vector2f{64.f, 0.f}));
        int deaths = 0;
        level.setPlayerDeathCallback([&](PlayerManager&) { ++deaths; });
        level.update(0.05f);
        passed &= check(!player.isDead() && deaths == 0,
                        "immortal player ignores harmful enemy contact");
    }

    {
        LevelManager level;
        Mario player;
        player.setHealth(1);
        player.setPosition({0.f, 0.f});
        player.getMovementComponent()->setVelocity(0.f, 100.f);
        level.setPlayers({&player});
        level.addEntity(std::make_unique<Heriss>(sf::Vector2f{0.f, 64.f}));
        int deaths = 0;
        level.setPlayerDeathCallback([&](PlayerManager&) { ++deaths; });
        level.update(0.05f);
        passed &= check(player.isDead() && deaths == 1,
                        "Heriss converts an apparent stomp into one harmful contact");
    }

    {
        LevelManager level;
        Mario player;
        player.setPosition({0.f, 0.f});
        player.getMovementComponent()->setVelocity(0.f, 100.f);
        level.setPlayers({&player});
        auto koopa = std::make_unique<Koopa>(sf::Vector2f{0.f, 64.f});
        Koopa* koopaView = koopa.get();
        level.addEntity(std::move(koopa));
        level.update(0.05f);

        player.setPosition({-60.f, koopaView->getPosition().y});
        player.getMovementComponent()->setVelocity(100.f, 0.f);
        level.update(0.05f);
        passed &= check(koopaView->isInShell() && koopaView->isShellKicked(),
                        "Koopa stomp and side contact transition shell once");
    }

    {
        LevelManager level;
        auto first = std::make_unique<Goomba>(sf::Vector2f{0.f, 0.f});
        auto second = std::make_unique<Goomba>(sf::Vector2f{48.f, 0.f});
        Goomba* firstView = first.get();
        Goomba* secondView = second.get();
        firstView->setFacingRight(true);
        secondView->setFacingRight(false);
        level.addEntity(std::move(first), true);
        level.addEntity(std::move(second), true);
        level.update(0.01f);
        level.update(0.05f);
        passed &= check(!firstView->isFacingRight() &&
                            secondView->isFacingRight() &&
                            firstView->getMovementComponent()->getVelocity().x < 0.f &&
                            secondView->getMovementComponent()->getVelocity().x > 0.f &&
                            !firstView->hitbox.getGlobalBounds().findIntersection(
                                secondView->hitbox.getGlobalBounds()),
                        "walking enemies separate and keep moving away on contact");
    }

    {
        LevelManager level;
        auto shell = std::make_unique<Koopa>(sf::Vector2f{0.f, 0.f});
        Koopa* shellView = shell.get();
        shellView->onStomped();
        shellView->kickShell(true);
        level.addEntity(std::move(shell), true);
        level.addEntity(std::make_unique<Goomba>(sf::Vector2f{32.f, 0.f}),
                        true);
        int score = 0;
        level.setScoreChangedCallback([&](int delta) { score += delta; });
        level.update(0.01f);
        passed &= check(level.getEnemies().size() == 1 &&
                            level.getEnemies().front() == shellView &&
                            score == 300,
                        "kicked Koopa shell defeats and removes one enemy");
    }

    {
        LevelManager level;
        Mario player;
        player.setPosition({0.f, 0.f});
        level.setPlayers({&player});
        level.addEntity(std::make_unique<Coin>(sf::Vector2f{0.f, 0.f}));
        int score = 0;
        int coins = 0;
        int coinCues = 0;
        level.setScoreChangedCallback([&](int delta) { score += delta; });
        level.setCoinCollectedCallback([&](int delta) { coins += delta; });
        level.setAudioCueCallback([&](AudioCue cue) {
            if (cue == AudioCue::Coin) ++coinCues;
        });
        level.update(0.01f);
        level.update(0.01f);
        passed &= check(score == 0 && coins == 1 && coinCues == 1 &&
                            level.getEntities().empty(),
                        "Coin collection forwards one outcome/cue and removes owner");
    }

    {
        Mario player;
        ConfiguredLevel level(1, 1, {&player});
        CoinBlock* coinBlock = nullptr;
        for (const auto& entity : level.getEntities()) {
            if (auto* candidate = dynamic_cast<CoinBlock*>(entity.get())) {
                coinBlock = candidate;
                break;
            }
        }
        int score = 0;
        int coins = 0;
        int cues = 0;
        level.setScoreChangedCallback([&](int delta) { score += delta; });
        level.setCoinCollectedCallback([&](int delta) { coins += delta; });
        level.setAudioCueCallback([&](AudioCue cue) {
            if (cue == AudioCue::Coin) ++cues;
        });
        const auto initialCoins = std::count_if(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<const Coin*>(entity.get()) != nullptr;
            });
        if (coinBlock) {
            const sf::FloatRect blockBounds = coinBlock->hitbox.getGlobalBounds();
            player.setPosition({blockBounds.position.x,
                                blockBounds.position.y + blockBounds.size.y});
            player.setGrounded(false);
            player.getMovementComponent()->setVelocity(0.f, -400.f);
        }
        level.update(0.05f);
        const auto poppedCoins = std::count_if(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<const Coin*>(entity.get()) != nullptr;
            });
        passed &= check(level.isLoaded() && coinBlock && score == 100 &&
                            coins == 1 && cues == 1 &&
                            poppedCoins == initialCoins + 1,
                        "CoinBlock immediately awards once and owns one visual coin");
    }

    {
        LevelManager level;
        Mario player;
        player.setPosition({0.f, 64.f});
        level.setPlayers({&player});
        level.addEntity(std::make_unique<TestBlock>(
                            sf::Vector2f{0.f, 0.f},
                            sf::Vector2f{64.f, 32.f}),
                        false, true);
        auto mushroom = std::make_unique<Mushroom>(sf::Vector2f{0.f, 64.f});
        Mushroom* mushroomView = mushroom.get();
        level.addEntity(std::move(mushroom));
        int score = 0;
        level.setScoreChangedCallback([&](int delta) { score += delta; });
        level.update(0.01f);
        passed &= check(!player.isBig() && mushroomView->exists() &&
                            score == 0,
                        "low ceiling denies Mushroom without consuming it");
    }

    {
        LevelManager level;
        Mario player;
        player.setPosition({0.f, 64.f});
        level.setPlayers({&player});
        level.addEntity(std::make_unique<Mushroom>(
            sf::Vector2f{0.f, 64.f}));
        int score = 0;
        level.setScoreChangedCallback([&](int delta) { score += delta; });
        level.update(0.01f);
        passed &= check(player.isBig() && score == 0 &&
                            level.getEntities().empty(),
                        "cleared Mushroom grows player and cleans item once");
    }

    {
        Mushroom mushroom({0.f, 64.f}, true);
        mushroom.update(0.25f);
        const bool roseDuringPop = mushroom.getPosition().y < 64.f;
        mushroom.update(0.25f);
        passed &= check(roseDuringPop &&
                            nearlyEqual(mushroom.getPosition().y, 64.f),
                        "popped Mushroom bobs once and settles at its spawn");
    }

    {
        LevelManager level;
        Mario target;
        target.setPosition({100.f, 300.f});
        level.setPlayers({&target});
        level.addEntity(std::make_unique<Bowser>(sf::Vector2f{500.f, 0.f}));
        level.update(2.01f);
        Bullet* aimed = nullptr;
        for (const auto& entity : level.getEntities()) {
            if (auto* bullet = dynamic_cast<Bullet*>(entity.get())) {
                aimed = bullet;
                break;
            }
        }
        const sf::Vector2f birth = aimed ? aimed->getPosition()
                                         : sf::Vector2f{};
        level.update(0.1f);
        passed &= check(
            aimed && aimed->getSize() ==
                         sf::Vector2f(MapFormat::TILE_SIZE,
                                      MapFormat::TILE_SIZE) &&
                aimed->getPosition().x < birth.x &&
                aimed->getPosition().y > birth.y &&
                !level.getEnemies().empty() &&
                !level.getEnemies().front()->isFacingRight(),
            "Bowser faces the live player and aims his 64px Bullet at them");
    }

    {
        LevelManager level;
        auto bowser = std::make_unique<Bowser>(sf::Vector2f{0.f, 0.f});
        level.addEntity(std::move(bowser));
        level.update(2.01f);
        Bullet* spawned = nullptr;
        for (const auto& entity : level.getEntities()) {
            if (auto* bullet = dynamic_cast<Bullet*>(entity.get())) {
                spawned = bullet;
                break;
            }
        }
        const float birthX = spawned ? spawned->getPosition().x : 0.f;
        const bool adoptedAfterTraversal =
            spawned && spawned->getDamage() == 2 && nearlyEqual(birthX, -32.f);
        level.update(0.1f);
        passed &= check(adoptedAfterTraversal &&
                            spawned->getPosition().x < birthX,
                        "base Enemy projectile request queues one damaging Bullet");
    }

    {
        LevelManager level;
        auto floor = std::make_unique<TestBlock>(
            sf::Vector2f{0.f, 64.f}, sf::Vector2f{256.f, 32.f});
        level.addEntity(std::move(floor), false, true);
        const bool accepted = level.spawnProjectile(
            {ProjectileKind::Fireball, {0.f, 0.f}, {1.f, 0.f}, 400.f, 1});
        level.update(0.2f);
        Fireball* bouncing = nullptr;
        for (const auto& entity : level.getEntities()) {
            if (auto* fireball = dynamic_cast<Fireball*>(entity.get())) {
                bouncing = fireball;
                break;
            }
        }
        passed &= check(
            accepted && bouncing && !bouncing->isExpired() &&
                nearlyEqual(bouncing->getPosition().y, 32.f) &&
                bouncing->getMovementComponent()->getVelocity().y < 0.f,
            "Fireball bounces only from a floor top");
    }

    {
        LevelManager level;
        auto wall = std::make_unique<TestBlock>(
            sf::Vector2f{64.f, 0.f}, sf::Vector2f{32.f, 256.f});
        level.addEntity(std::move(wall), false, true);
        const bool accepted = level.spawnProjectile(
            {ProjectileKind::Fireball, {0.f, 0.f}, {1.f, 0.f}, 400.f, 1});
        level.update(0.1f);
        const bool removed = std::none_of(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<Fireball*>(entity.get()) != nullptr;
            });
        passed &= check(accepted && removed,
                        "Fireball expires instead of sticking in a wall");
    }

    {
        LevelManager level;
        ProjectileSpawnRequest invalid{ProjectileKind::BowserFire,
                                       {0.f, 0.f}, {0.f, 0.f}, 350.f, 2};
        ProjectileSpawnRequest unsupportedFireballDamage{
            ProjectileKind::Fireball, {0.f, 0.f}, {1.f, 0.f}, 400.f, 2};
        const bool rejected = !level.spawnProjectile(invalid) &&
                              !level.spawnProjectile(
                                  unsupportedFireballDamage);
        ProjectileSpawnRequest fireball{ProjectileKind::Fireball,
                                        {0.f, 0.f}, {1.f, 0.f}, 200.f, 1};
        const bool accepted = level.spawnProjectile(fireball);
        level.addEntity(std::make_unique<TestEnemy>(sf::Vector2f{48.f, 0.f}));
        int score = 0;
        level.setScoreChangedCallback([&](int delta) { score += delta; });
        level.update(0.1f);
        const bool noProjectileOrEnemy = std::none_of(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<Fireball*>(entity.get()) != nullptr ||
                       dynamic_cast<Enemy*>(entity.get()) != nullptr;
            });
        passed &= check(rejected && accepted && noProjectileOrEnemy &&
                            score == 100,
                        "Fireball request rejects invalid data and resolves one enemy hit");
    }

    {
        LevelManager level;
        const bool accepted = level.spawnProjectile(
            {ProjectileKind::Fireball, {32.f, 32.f},
             {0.f, -2.f}, 750.f, 1});
        Fireball* fireball = nullptr;
        for (const auto& entity : level.getEntities()) {
            if (auto* candidate = dynamic_cast<Fireball*>(entity.get())) {
                fireball = candidate;
                break;
            }
        }
        passed &= check(
            accepted && fireball &&
                fireball->getMovementComponent()->getVelocity() ==
                    sf::Vector2f(0.f, -750.f),
            "Mouse-aimed Fireball accepts and normalizes a vertical shot");
    }

    {
        Mario player;
        ConfiguredLevel level(1, 1, {&player});
        std::vector<sf::Vector2f> peteyPositions;
        for (Enemy* enemy : level.getEnemies()) {
            if (dynamic_cast<PeteyPiranha*>(enemy))
                peteyPositions.push_back(enemy->getPosition());
        }
        level.update(2.01f);
        std::size_t anchoredPeteys = 0;
        for (Enemy* enemy : level.getEnemies()) {
            if (auto* petey = dynamic_cast<PeteyPiranha*>(enemy)) {
                if (anchoredPeteys < peteyPositions.size() &&
                    petey->getPosition() == peteyPositions[anchoredPeteys]) {
                    ++anchoredPeteys;
                }
            }
        }
        const auto bulletCount = static_cast<std::size_t>(std::count_if(
            level.getEntities().begin(), level.getEntities().end(),
            [](const std::unique_ptr<GameObject>& entity) {
                return dynamic_cast<const Bullet*>(entity.get()) != nullptr;
            }));
        passed &= check(!peteyPositions.empty() &&
                            anchoredPeteys == peteyPositions.size() &&
                            bulletCount == peteyPositions.size(),
                        "W1_LV1 Peteys stay anchored and each open with one upward shot");
    }

    {
        W3_LV3 level;
        const auto bounds = level.getWorldBounds();
        const float farRight = bounds
            ? bounds->position.x + bounds->size.x +
                  2.f * MapFormat::TILE_SIZE
            : 4096.f;
        auto offWorldEnemy =
            std::make_unique<TestEnemy>(sf::Vector2f{farRight, 64.f});
        TestEnemy* offWorldEnemyView = offWorldEnemy.get();
        level.addEntity(std::move(offWorldEnemy));
        auto airborneEnemy =
            std::make_unique<TestEnemy>(sf::Vector2f{64.f, -640.f});
        TestEnemy* airborneEnemyView = airborneEnemy.get();
        level.addEntity(std::move(airborneEnemy));
        auto offWorldCoin =
            std::make_unique<Coin>(sf::Vector2f{farRight, 64.f});
        Coin* offWorldCoinView = offWorldCoin.get();
        level.addEntity(std::move(offWorldCoin));
        level.spawnProjectile({ProjectileKind::Fireball,
                               {farRight, 128.f}, {1.f, 0.f}, 400.f, 1});
        level.update(0.01f);

        const bool offWorldOwnersRemoved = std::none_of(
            level.getEntities().begin(), level.getEntities().end(),
            [offWorldEnemyView, offWorldCoinView](
                const std::unique_ptr<GameObject>& entity) {
                return entity.get() == offWorldEnemyView ||
                       entity.get() == offWorldCoinView ||
                       (dynamic_cast<Fireball*>(entity.get()) != nullptr &&
                        entity->getPosition().x > 1600.f);
            });
        const bool airborneRetained =
            std::find(level.getEnemies().begin(), level.getEnemies().end(),
                      airborneEnemyView) != level.getEnemies().end();
        passed &= check(bounds && offWorldOwnersRemoved && airborneRetained,
                        "world cleanup removes actors/items/fireballs but retains airborne enemy");
    }

    {
        PhysicsEngine physics(980.f);
        Mario player;
        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);

        std::vector<std::unique_ptr<GameObject>> fragments;
        std::unordered_set<GameObject*> delivered;
        int callbackCalls = 0;
        Brick brick([&](std::unique_ptr<GameObject> spawned) {
            ++callbackCalls;
            if (spawned) {
                delivered.insert(spawned.get());
                fragments.push_back(std::move(spawned));
            }
        });
        brick.setPosition({64.f, 64.f});
        brick.setSize({32.f, 32.f});

        const CollisionInfo collision = physics.step(player, {&brick}, 0.05f);
        passed &= check(collision.ceilHit,
                        "real player headbutt reaches normal Brick bottom");
        passed &= check(brick.isExist() && callbackCalls == 0,
                        "small player cannot break a Brick");

        player.setBig(true);
        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);
        physics.step(player, {&brick}, 0.05f);
        passed &= check(!brick.isExist(),
                        "big player breaks a Brick from below");

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
        player.setBig(true);
        player.setPosition({64.f, 98.f});
        player.getMovementComponent()->setVelocity(0.f, -200.f);
        player.setGrounded(false);
        sameFrameFollower.setPosition({64.f, 98.f});
        sameFrameFollower.getMovementComponent()->setVelocity(0.f, -200.f);
        sameFrameFollower.setGrounded(false);
        level.setPlayers({&player, &sameFrameFollower});

        int callbackCalls = 0;
        int brickBreakCues = 0;
        int brickScore = 0;
        std::unordered_set<GameObject*> delivered;
        auto brick = std::make_unique<Brick>([&](std::unique_ptr<GameObject> spawned) {
            ++callbackCalls;
            if (spawned) delivered.insert(spawned.get());
            level.addEntity(std::move(spawned));
        });
        brick->setPosition({64.f, 64.f});
        brick->setSize({32.f, 32.f});
        level.addEntity(std::move(brick), false, true);
        level.setAudioCueCallback([&](AudioCue cue) {
            if (cue == AudioCue::BrickBreak) ++brickBreakCues;
        });
        level.setScoreChangedCallback([&](int delta) { brickScore += delta; });

        level.update(0.05f);

        passed &= check(callbackCalls == 4 &&
                            player.getPosition().y >= 96.f &&
                            player.getMovementComponent()->getVelocity().y > 0.f,
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
                            brickBreakCues == 1 && brickScore == 50 &&
                            fragmentAddresses.size() == 4,
                        "LevelManager owns fragments and rewards one Brick break");
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

        struct FactoryExpectation {
            int world;
            int stage;
            std::size_t extraEnemies;
            std::size_t movingBlocks;
            std::size_t winFlags;
        };
        constexpr std::array<FactoryExpectation, 9> factories{{
            {1, 1, 20, 0, 1}, {1, 2, 16, 1, 1},
            {1, 3, 18, 7, 1}, {2, 1, 19, 1, 1},
            {2, 2, 14, 2, 1}, {2, 3, 26, 1, 1},
            {3, 1, 9, 0, 1},  {3, 2, 14, 0, 1},
            {3, 3, 1, 0, 0},
        }};
        for (const FactoryExpectation& expected : factories) {
            auto player = std::make_unique<Mario>();
            auto level = createConfiguredLevel(
                expected.world, expected.stage, {player.get()});
            const std::size_t mapEnemyCount = level
                ? static_cast<std::size_t>(std::count_if(
                      level->getMapManager().getSpawns().begin(),
                      level->getMapManager().getSpawns().end(),
                      [](const MapSpawnInfo& spawn) {
                          return spawn.type == MapObjectType::Goomba ||
                                 spawn.type == MapObjectType::Koopa ||
                                 spawn.type == MapObjectType::FlyingKoopa ||
                                 spawn.type == MapObjectType::Heriss ||
                                 spawn.type == MapObjectType::PeteyPiranha ||
                                 spawn.type == MapObjectType::Bowser;
                      }))
                : 0;
            std::size_t movingBlocks = 0;
            std::size_t winFlags = 0;
            bool spawnSafe = level && level->isLoaded();
            if (level) {
                const sf::FloatRect playerBounds =
                    player->hitbox.getGlobalBounds();
                for (const auto& entity : level->getEntities()) {
                    if (dynamic_cast<MovingBlock*>(entity.get()))
                        ++movingBlocks;
                    if (dynamic_cast<WinFlag*>(entity.get())) ++winFlags;
                    if ((dynamic_cast<Enemy*>(entity.get()) ||
                         dynamic_cast<Lava*>(entity.get())) &&
                        playerBounds.findIntersection(
                            entity->hitbox.getGlobalBounds())) {
                        spawnSafe = false;
                    }
                }
                level->update(0.016f);
                spawnSafe = spawnSafe && !player->isDead();
            }
            const auto primarySpawn = level
                ? std::find_if(
                      level->getMapManager().getSpawns().begin(),
                      level->getMapManager().getSpawns().end(),
                      [](const MapSpawnInfo& spawn) {
                          return spawn.type == MapObjectType::Player1Spawn;
                      })
                : std::vector<MapSpawnInfo>::const_iterator{};
            auto luigi = std::make_unique<Luigi>();
            auto luigiLevel = createConfiguredLevel(
                expected.world, expected.stage, {luigi.get()});
            const bool selectedCharacterUsesPrimarySpawn =
                level && luigiLevel && luigiLevel->isLoaded() &&
                primarySpawn != level->getMapManager().getSpawns().end() &&
                luigi->getPosition() == primarySpawn->position;
            passed &= check(
                level && level->getEnemies().size() ==
                             mapEnemyCount + expected.extraEnemies &&
                    movingBlocks == expected.movingBlocks &&
                    winFlags == expected.winFlags && spawnSafe &&
                    selectedCharacterUsesPrimarySpawn,
                "production W" + std::to_string(expected.world) + "_LV" +
                    std::to_string(expected.stage) +
                    " factory preserves Group5 extras and a safe spawn" +
                    (level ? " [enemies " +
                                 std::to_string(level->getEnemies().size()) +
                                 "/" + std::to_string(mapEnemyCount +
                                                        expected.extraEnemies) +
                                 ", moving " + std::to_string(movingBlocks) +
                                 "/" + std::to_string(expected.movingBlocks) +
                                 ", flags " + std::to_string(winFlags) +
                                 "/" + std::to_string(expected.winFlags) +
                                  ", safe " + (spawnSafe ? "yes" : "no") +
                                  ", Luigi primary " +
                                  (selectedCharacterUsesPrimarySpawn
                                       ? "yes"
                                       : "no") +
                                 ", spawn " +
                                 std::to_string(static_cast<int>(
                                     player->getPosition().x)) + "," +
                                 std::to_string(static_cast<int>(
                                     player->getPosition().y)) + "]"
                           : " [level missing]"));
        }

        for (int world = 1; world <= 3; ++world) {
            for (int stage = 1; stage <= 3; ++stage) {
                if (world == 3 && stage == 3) continue;
                Mario finisher;
                finisher.setImmortal(true, 10.f);
                ConfiguredLevel completable(world, stage, {&finisher});
                std::vector<WinFlag*> flags;
                for (const auto& entity : completable.getEntities()) {
                    if (auto* flag = dynamic_cast<WinFlag*>(entity.get()))
                        flags.push_back(flag);
                }
                int completions = 0;
                completable.setLevelCompletedCallback([&] { ++completions; });
                if (flags.size() == 1)
                    finisher.setPosition(flags.front()->hitbox.getPosition());
                completable.update(0.01f);
                const bool activatedByPlayer =
                    flags.size() == 1 && flags.front()->isActivated();
                completable.update(0.5f);
                completable.update(0.5f);
                completable.update(0.1f);
                passed &= check(
                    completable.isLoaded() && flags.size() == 1 &&
                        activatedByPlayer && completions == 1,
                    "W" + std::to_string(world) + "_LV" +
                        std::to_string(stage) +
                        " exposes one player-contact WinFlag completion path");
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

        Mario bossPlayer;
        W3_LV3 poweredBossLevel({&bossPlayer});
        poweredBossLevel.update(5.01f);
        Mushroom* bossMushroom = nullptr;
        for (const auto& entity : poweredBossLevel.getEntities()) {
            if (auto* mushroom = dynamic_cast<Mushroom*>(entity.get())) {
                bossMushroom = mushroom;
                break;
            }
        }
        if (bossMushroom)
            bossPlayer.setPosition(bossMushroom->hitbox.getPosition());
        poweredBossLevel.update(0.01f);
        passed &= check(
            bossMushroom && bossPlayer.isBig() && bossPlayer.canShoot(),
            "W3_LV3 type-2 mushroom grows the player and unlocks FireBuff shooting");

        int bossCompletionCalls = 0;
        level.setLevelCompletedCallback([&] { ++bossCompletionCalls; });
        if (bowser != level.getEnemies().end())
            (*bowser)->takeDamage((*bowser)->getHealth());
        level.update(0.01f);
        level.update(0.01f);
        passed &= check(bossCompletionCalls == 1,
                        "defeating Bowser completes W3_LV3 exactly once");
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
