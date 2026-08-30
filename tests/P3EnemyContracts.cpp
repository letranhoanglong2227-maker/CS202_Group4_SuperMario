#include "Entities/Enemies/Goomba.hpp"
#include "Entities/Enemies/Koopa.hpp"
#include "Entities/Enemies/Heriss.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Entities/Players/Mario.hpp"
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
    checkP3(proj.has_value() && proj->type == "BowserFire", "Bowser projectile consumed");
    checkP3(!bowser.hasPendingProjectile(), "Pending projectile cleared after consumption");

    // Enemy lifecycle (predicate = isDead())
    checkP3(bowser.isDead() == false, "Bowser is alive initially");
    bowser.takeDamage(9999);
    checkP3(bowser.isDead() == true, "Enemy isdDead() == true when health <= 0, ready for removal");

    std::cout << "P3 Enemy Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
