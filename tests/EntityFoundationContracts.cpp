#include "Entities/Base/Enemy.hpp"
#include "Entities/Players/Mario.hpp"
#include "Core/Constants.hpp"
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
    std::cout << "--- Entity Foundation Contracts ---\n";

    Mario mario;
    Enemy enemy;

    checkP3(mario.getHealth() > 0, "Character initialized with positive health");
    checkP3(enemy.getPointsValue() == 100, "Base enemy has default points");

    enemy.takeDamage(999);
    checkP3(enemy.isDead() == true, "Enemy dies when health <= 0");

    auto out = enemy.handlePlayerContact(mario, PlayerEnemyContactKind::Stomp, 0.f);
    checkP3(out.result == EnemyContactResult::None, "Dead enemy returns None contact outcome");

    std::cout << "Entity Foundation Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
