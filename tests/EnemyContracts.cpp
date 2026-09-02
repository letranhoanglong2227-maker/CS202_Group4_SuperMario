#include "Entities/Enemies/Goomba.hpp"
#include "Entities/Enemies/Koopa.hpp"
#include "Entities/Enemies/Heriss.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Entities/Players/Mario.hpp"
#include "Entities/Enemies/PeteyPiranha.hpp"
#include "Entities/Enemies/FlyingKoopa.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <cmath>
#include <iostream>
#include <string>

int P3_test_passed = 0;
int P3_test_failed = 0;

void checkP3(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASS] " << testName << "\n";
        P3_test_passed++;
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        P3_test_failed++;
    }
}

class InspectableFlyingKoopa : public FlyingKoopa {
public:
    using FlyingKoopa::FlyingKoopa;
    sf::IntRect frame() const { return entitySprite.getTextureRect(); }
    sf::Vector2f visualScale() const { return entitySprite.getScale(); }
};

class InspectableGoomba : public Goomba {
public:
    using Goomba::Goomba;
    sf::FloatRect visualBounds() const { return entitySprite.getGlobalBounds(); }
    sf::Vector2f visualScale() const { return entitySprite.getScale(); }
};

class InspectablePetey : public PeteyPiranha {
public:
    using PeteyPiranha::PeteyPiranha;
    sf::IntRect frame() const { return entitySprite.getTextureRect(); }
    sf::Vector2f visualScale() const { return entitySprite.getScale(); }
    sf::FloatRect visualBounds() const { return entitySprite.getGlobalBounds(); }
};

int main() {
    std::cout << "--- Enemy Contracts ---\n";
    Mario player;
    checkP3(TextureEnemyManager::setupTexture(),
            "Enemy texture provider is ready for production rendering");

    InspectableFlyingKoopa flying({500.f, 200.f});
    flying.update(0.016f);
    checkP3(flying.frame().position.x == 86 &&
                flying.frame().size.x == 16 && flying.frame().size.y == 24,
            "FlyingKoopa selects a wing frame instead of the full atlas");
    sf::RenderTexture flyingTarget({320u, 180u});
    flying.render(&flyingTarget);
    checkP3(std::abs(std::abs(flying.visualScale().x) - 4.f) < 0.001f &&
                std::abs(flying.visualScale().y - 4.f) < 0.001f,
            "FlyingKoopa visual size matches its 64x96 hitbox");
    for (int frame = 0; frame < 500; ++frame) flying.update(0.016f);
    checkP3(flying.getPosition().x >= 200.f && flying.getPosition().x <= 800.f,
            "FlyingKoopa stays inside its spawn patrol range");

    Goomba movingGoomba({0.f, 0.f});
    Koopa movingKoopa({0.f, 0.f});
    Heriss movingHeriss({0.f, 0.f});
    movingGoomba.setFacingRight(true);
    movingKoopa.setFacingRight(true);
    movingHeriss.setFacingRight(true);
    movingGoomba.update(0.2f);
    movingKoopa.update(0.2f);
    movingHeriss.update(0.2f);
    checkP3(
        movingGoomba.getMovementComponent()->getVelocity().x == 300.f &&
            movingKoopa.getMovementComponent()->getVelocity().x == 300.f &&
            movingHeriss.getMovementComponent()->getVelocity().x == 300.f &&
            flying.getSpeed() == 300.f,
        "walking enemies use Group5's effective 300px/s movement cap");
    checkP3(movingGoomba.getPointsValue() == 300 &&
                movingHeriss.getPointsValue() == 300 &&
                flying.getPointsValue() == 400,
            "enemy defeat rewards match Group5");

    // Goomba
    Goomba goomba({0.f, 0.f});
    auto goomRes = goomba.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f); // Stomp
    checkP3(goomRes.result == EnemyContactResult::EnemyStomped && goomRes.scoreDelta == 300, "Goomba stomped gives Group5 score and result");

    auto goomRes2 = goomba.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f);
    checkP3(goomRes2.result == EnemyContactResult::None && goomRes2.scoreDelta == 0, "Goomba stomped twice gives no score");

    InspectableGoomba squishedVisual({100.f, 200.f});
    squishedVisual.setFacingRight(true);
    squishedVisual.update(0.2f);
    checkP3(squishedVisual.getMovementComponent()->getVelocity().x == 300.f,
            "Goomba is moving before the stomp contract");
    squishedVisual.handlePlayerContact(
        player, PlayerEnemyContactKind::Stomp, 0.f);
    checkP3(squishedVisual.getMovementComponent() &&
                squishedVisual.getMovementComponent()->getVelocity() ==
                    sf::Vector2f(0.f, 0.f),
            "Squished Goomba stops moving immediately");
    squishedVisual.update(0.f);
    sf::RenderTexture squishedTarget({320u, 180u});
    squishedVisual.render(&squishedTarget);
    checkP3(squishedVisual.getSize() == sf::Vector2f(64.f, 32.f) &&
                squishedVisual.visualScale() == sf::Vector2f(4.f, 4.f) &&
                squishedVisual.visualBounds().size == sf::Vector2f(64.f, 32.f),
            "Squished Goomba keeps a 4x frame and 64x32 bounds");

    // Koopa
    Koopa koopa({0.f, 0.f});
    auto koopaRes1 = koopa.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f); // Stomp -> Shell
    checkP3(koopaRes1.result == EnemyContactResult::EnemyStomped && koopaRes1.scoreDelta == 200, "Koopa stomped into shell");
    checkP3(koopa.isInShell(), "Koopa is in shell state");
    checkP3(koopa.getMovementComponent() &&
                koopa.isShellKicked() &&
                std::abs(koopa.getMovementComponent()->getVelocity().x) == 300.f,
            "First stomp launches Koopa shell in its walking direction");

    auto koopaRes2 = koopa.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 1.f);
    checkP3(koopaRes2.result == EnemyContactResult::ShellStopped &&
                !koopa.isShellKicked(),
            "A second stomp safely stops a moving Koopa shell");

    Koopa regrowingKoopa({0.f, 0.f});
    const float regrowBottom = regrowingKoopa.getPosition().y +
                               regrowingKoopa.hitbox.getSize().y;
    regrowingKoopa.onStomped();
    regrowingKoopa.update(5.01f);
    checkP3(!regrowingKoopa.isInShell() &&
                std::abs(regrowingKoopa.hitbox.getPosition().y +
                         regrowingKoopa.hitbox.getSize().y - regrowBottom) < 0.001f,
            "Koopa exits an idle shell after 5 seconds with feet anchored");

    // Heriss
    Heriss heriss({0.f, 0.f});
    auto herissRes = heriss.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f); // Stomp
    checkP3(herissRes.result == EnemyContactResult::PlayerDamaged || herissRes.result == EnemyContactResult::PlayerKilled, "Heriss damages player even when stomped");

    // Bowser
    Bowser bowser({0.f, 0.f});
    checkP3(bowser.getHealth() == 30 && bowser.getSpeed() == 300.f,
            "Bowser keeps Group5's 30 HP and effective movement cap");
    bowser.update(2.5f); // Advance timer to fire breath
    checkP3(bowser.hasPendingProjectile(), "Bowser has deterministic pending projectile");
    auto proj = bowser.consumePendingProjectile();
    checkP3(proj.has_value() && proj->type == ProjectileKind::BowserFire &&
                proj->speed == 500.f,
            "Bowser projectile keeps Group5's 500px/s speed");
    checkP3(!bowser.hasPendingProjectile(), "Pending projectile cleared after consumption");

    // Immortal side contact
    Mario player2;
    player2.setBig(true);
    player2.takeDamage(1);
    Goomba goomba2({0.f, 0.f});
    auto immortalRes = goomba2.handlePlayerContact(player2, PlayerEnemyContactKind::Side, 0.f);
    checkP3(immortalRes.result == EnemyContactResult::None, "Immortal side contact returns None");

    // Hitbox anchors
    Goomba goomba3({0.f, 0.f});
    float g3Bot = goomba3.getPosition().y + goomba3.hitbox.getSize().y;
    goomba3.handlePlayerContact(player2, PlayerEnemyContactKind::Stomp, 0.f);
    float g3BotNew = goomba3.hitbox.getPosition().y + goomba3.hitbox.getSize().y;
    checkP3(g3Bot == g3BotNew, "Goomba hitbox anchor maintained");

    Koopa koopa3({0.f, 0.f});
    float k3Bot = koopa3.getPosition().y + koopa3.hitbox.getSize().y;
    koopa3.handlePlayerContact(player2, PlayerEnemyContactKind::Stomp, 0.f);
    float k3BotNew = koopa3.hitbox.getPosition().y + koopa3.hitbox.getSize().y;
    checkP3(k3Bot == k3BotNew, "Koopa hitbox anchor maintained");

    // Petey
    InspectablePetey petey({0.f, 0.f});
    const sf::Vector2f peteyPosition = petey.getPosition();
    petey.update(0.f);
    sf::RenderTexture peteyTarget({320u, 180u});
    petey.render(&peteyTarget);
    checkP3(petey.frame() == sf::IntRect({222, 37}, {16, 24}) &&
                petey.visualScale() == sf::Vector2f(4.f, 4.f) &&
                petey.visualBounds().size == sf::Vector2f(64.f, 96.f),
            "Petey closed frame, scale and bounds match Group5");
    petey.update(2.01f);
    checkP3(petey.isEmerged() && petey.getPosition() == peteyPosition &&
                petey.frame() == sf::IntRect({205, 37}, {16, 24}),
            "Petey opens its Group5 frame without leaving its map anchor");
    auto pProj = petey.consumePendingProjectile();
    checkP3(!petey.hasPendingProjectile() && pProj.has_value() &&
                pProj->type == ProjectileKind::PeteySpike &&
                pProj->direction == sf::Vector2f(0.f, -1.f) &&
                pProj->speed == 400.f,
            "Petey opens and queues one visible upward fire shot");

    // Timing boundary
    Bowser bowser2({0.f, 0.f});
    bowser2.update(1.99f);
    checkP3(!bowser2.hasPendingProjectile(), "Bowser boundary: not fired at 1.99s");
    bowser2.update(0.02f);
    checkP3(bowser2.hasPendingProjectile(), "Bowser boundary: fired at 2.01s");

    // Enemy lifecycle (predicate = isDead())
    checkP3(bowser.isDead() == false, "Bowser is alive initially");
    bowser.takeDamage(9999);
    checkP3(bowser.isDead() == true, "Enemy isdDead() == true when health <= 0, ready for removal");

    std::cout << "Enemy Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
