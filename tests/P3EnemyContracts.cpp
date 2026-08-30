#include "Entities/Enemies/Goomba.hpp"
#include "Entities/Enemies/Koopa.hpp"
#include "Entities/Enemies/Heriss.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Entities/Players/Mario.hpp"
#include "Entities/Enemies/PeteyPiranha.hpp"
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

int main() {
    std::cout << "--- P3 Enemy Contracts ---\n";
    Mario player;

    // Goomba
    Goomba goomba({0.f, 0.f});
    auto goomRes = goomba.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f); // Stomp
    checkP3(goomRes.result == EnemyContactResult::EnemyStomped && goomRes.scoreDelta == 100, "Goomba stomped gives score and result");

    auto goomRes2 = goomba.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f);
    checkP3(goomRes2.result == EnemyContactResult::None && goomRes2.scoreDelta == 0, "Goomba stomped twice gives no score");

    // Koopa
    Koopa koopa({0.f, 0.f});
    auto koopaRes1 = koopa.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f); // Stomp -> Shell
    checkP3(koopaRes1.result == EnemyContactResult::EnemyStomped && koopaRes1.scoreDelta == 200, "Koopa stomped into shell");
    checkP3(koopa.isInShell(), "Koopa is in shell state");

    auto koopaRes2 = koopa.handlePlayerContact(player, PlayerEnemyContactKind::Side, 1.f); // Side hit -> Kick
    checkP3(koopaRes2.result == EnemyContactResult::ShellKicked && koopa.isShellKicked(), "Koopa shell kicked");

    // Heriss
    Heriss heriss({0.f, 0.f});
    auto herissRes = heriss.handlePlayerContact(player, PlayerEnemyContactKind::Stomp, 0.f); // Stomp
    checkP3(herissRes.result == EnemyContactResult::PlayerDamaged || herissRes.result == EnemyContactResult::PlayerKilled, "Heriss damages player even when stomped");

    // Bowser
    Bowser bowser({0.f, 0.f});
    bowser.update(2.5f); // Advance timer to fire breath
    checkP3(bowser.hasPendingProjectile(), "Bowser has deterministic pending projectile");
    auto proj = bowser.consumePendingProjectile();
    checkP3(proj.has_value() && proj->type == ProjectileKind::BowserFire, "Bowser projectile consumed");
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
    PeteyPiranha petey({0.f, 0.f});
    petey.update(2.5f); petey.update(1.01f);
    checkP3(petey.hasPendingProjectile(), "Petey has pending projectile");
    auto pProj = petey.consumePendingProjectile();
    checkP3(pProj.has_value() && pProj->type == ProjectileKind::PeteySpike, "Petey projectile consumed");
    auto pProj2 = petey.consumePendingProjectile();
    checkP3(!pProj2.has_value(), "Petey consume request dung mot lan");

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

    std::cout << "P3 Enemy Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
