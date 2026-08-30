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
    std::cout << "--- P3 Player Contracts ---\n";
    Mario mario;
    mario.setPosition({100.f, 200.f}); // Bottom is 200 + 64 = 264
    
    // 1. State logic
    checkP3(mario.setBig(true, false) == false, "Growth rejected if canGrow=false");
    
    mario.setBig(true, true);
    checkP3(mario.isBig() == true, "Mario is Big");
    // Feet anchor check
    checkP3(mario.getPosition().y == 136.f, "Feet anchor preserved when growing (200 - 64)");
    
    mario.setFire(true, true);
    checkP3(mario.isFire() == true && mario.isBig() == true, "Fire form implies Big form");
    checkP3(mario.canShoot() == true, "Fire Mario can shoot");
    
    auto req = mario.shoot(1.f);
    checkP3(req.has_value() && req->type == "Fireball", "Shoot returns Fireball request");
    checkP3(mario.canShoot() == false, "Cannot shoot again immediately (cooldown)");
    
    // 2. Damage flow
    mario.takeDamage(1);
    checkP3(mario.isFire() == false && mario.isBig() == true, "Damage in Fire form drops to Big form");
    checkP3(mario.isImmortal() == true, "Damage grants immortality");
    
    mario.setImmortal(false);
    mario.takeDamage(1);
    checkP3(mario.isBig() == false && mario.isDead() == false, "Damage in Big form drops to Small form");
    checkP3(mario.getPosition().y == 200.f, "Feet anchor preserved when shrinking (136 + 64)");
    
    mario.setImmortal(false);
    mario.takeDamage(1);
    checkP3(mario.isDead() == true, "Damage in Small form kills player");
    
    // 3. Reset logic
    mario.resetForRespawn({50.f, 50.f});
    checkP3(mario.isDead() == false && mario.isBig() == false && mario.getPosition().x == 50.f, "resetForRespawn clears states and sets position");
    checkP3(mario.isImmortal() == false, "resetForRespawn clears immortality");
    checkP3(mario.isFire() == false, "resetForRespawn clears fire form");
    
    std::cout << "P3 Player Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
