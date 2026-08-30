#include "Entities/EntityFactory.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Objects/Items/PowerUpObject.hpp"
#include "Objects/Items/Fireball.hpp"
#include <iostream>
#include <memory>
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
    std::cout << "--- P3 Item & Factory Contracts ---\n";

    // 1. Factory tests
    auto player1 = EntityFactory::createPlayer("Mario", {10.f, 20.f});
    checkP3(player1 != nullptr && player1->getPosition().x == 10.f, "Factory creates Mario with correct position");

    auto player2 = EntityFactory::createPlayer("Unknown", {0.f, 0.f});
    checkP3(player2 == nullptr, "Factory returns nullptr for unknown player (no fallback)");

    auto enemy1 = EntityFactory::createEnemy("Goomba");
    checkP3(enemy1 != nullptr, "Factory creates Goomba");

    auto enemy2 = EntityFactory::createEnemy("Dragon");
    checkP3(enemy2 == nullptr, "Factory returns nullptr for unknown enemy (no fallback)");

    auto item1 = EntityFactory::createItem("FireFlower");
    checkP3(item1 != nullptr, "Factory creates FireFlower");

    auto item2 = EntityFactory::createItem("Star");
    checkP3(item2 == nullptr, "Factory returns nullptr for unknown item (no fallback)");

    // 2. Item collection exactly-once
    auto mushroom = EntityFactory::createItem("Mushroom");
    auto res1 = mushroom->collect(*player1);
    checkP3(res1.consumed == true && res1.requestedForm == RequestedPlayerForm::Big, "Mushroom collected first time");

    auto res2 = mushroom->collect(*player1);
    checkP3(res2.consumed == false && res2.scoreDelta == 0, "Mushroom cannot be collected twice");
    checkP3(mushroom->exists() == false, "Item exists() == false after collection, ready for removal");

    auto deniedMushroom = EntityFactory::createItem("Mushroom");
    auto deniedMushroomResult = deniedMushroom->collect(*player1, false);
    checkP3(!deniedMushroomResult.consumed && deniedMushroomResult.requestedForm == RequestedPlayerForm::None
                && deniedMushroom->exists(),
            "Denied Mushroom collection keeps item alive");

    auto deniedFireFlower = EntityFactory::createItem("FireFlower");
    auto deniedFireFlowerResult = deniedFireFlower->collect(*player1, false);
    checkP3(!deniedFireFlowerResult.consumed && deniedFireFlowerResult.requestedForm == RequestedPlayerForm::None
                && deniedFireFlower->exists(),
            "Denied FireFlower collection keeps item alive");

    auto acceptedFireFlowerResult = deniedFireFlower->collect(*player1, true);
    checkP3(acceptedFireFlowerResult.consumed && acceptedFireFlowerResult.requestedForm == RequestedPlayerForm::Fire
                && !deniedFireFlower->exists(),
            "FireFlower is consumed once after clearance succeeds");

    // 3. Fireball lifecycle
    Fireball fireball({0.f, 0.f}, 1.f);
    checkP3(!fireball.isExpired(), "Fireball is not expired initially");
    fireball.update(99.f); // Exceed maxLife (3.f)
    checkP3(fireball.isExpired() && fireball.canBeRemoved(), "Fireball is expired and can be removed after maxLife");

    std::cout << "P3 ItemFactory Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
