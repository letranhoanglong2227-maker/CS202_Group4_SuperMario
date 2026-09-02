#include "Entities/EntityFactory.hpp"
#include "Entities/Players/PlayerManager.hpp"
#include "Objects/Items/PowerUpObject.hpp"
#include "Objects/Items/Fireball.hpp"
#include "Objects/Items/Coin.hpp"
#include "Objects/Items/FireFlower.hpp"
#include "Objects/Items/Mushroom.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>
#include <memory>
#include <string>

int P3_test_passed = 0;
int P3_test_failed = 0;

class InspectableCoin final : public Coin {
public:
    using Coin::Coin;
    sf::FloatRect visualBounds() const { return entitySprite.getGlobalBounds(); }
};

class InspectableMushroom final : public Mushroom {
public:
    using Mushroom::Mushroom;
    sf::FloatRect visualBounds() const { return entitySprite.getGlobalBounds(); }
};

class InspectableFireball final : public Fireball {
public:
    using Fireball::Fireball;
    sf::FloatRect visualBounds() const { return entitySprite.getGlobalBounds(); }
};

class InspectableFireFlower final : public FireFlower {
public:
    using FireFlower::FireFlower;
    sf::IntRect visualFrame() const { return entitySprite.getTextureRect(); }
};

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

    const int healthBeforeHealing = player1->getHealth();
    Mushroom healingMushroom({0.f, 0.f}, false, 1);
    const auto healingResult = healingMushroom.collect(*player1);
    Mushroom strongHealingMushroom({0.f, 0.f}, false, 3);
    const auto strongHealingResult = strongHealingMushroom.collect(*player1);
    checkP3(healingResult.consumed && strongHealingResult.consumed &&
                healingResult.requestedForm == RequestedPlayerForm::None &&
                strongHealingResult.requestedForm == RequestedPlayerForm::None &&
                player1->getHealth() == healthBeforeHealing + 3,
            "Group5 mushroom types 1 and 3 heal while type 2 requests Big form");

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

    sf::RenderTexture visualTarget({320u, 180u});
    InspectableCoin visualCoin({0.f, 0.f});
    visualCoin.render(&visualTarget);
    InspectableMushroom visualMushroom({0.f, 0.f}, false, 1);
    visualMushroom.render(&visualTarget);
    InspectableFireball visualFireball({0.f, 0.f}, 1.f);
    visualFireball.render(&visualTarget);
    InspectableFireFlower visualFlower({0.f, 0.f});
    checkP3(visualCoin.visualBounds().size == sf::Vector2f(54.f, 64.f) &&
                visualMushroom.visualBounds().size == sf::Vector2f(64.f, 64.f) &&
                visualFireball.visualBounds().size == sf::Vector2f(32.f, 32.f),
            "Item and Fireball atlas frames fill their gameplay bounds");
    checkP3(visualFlower.visualFrame().position == sf::Vector2i(2, 19),
            "FireFlower starts on the flower atlas row instead of the Star row");

    // 3. Fireball lifecycle
    Fireball fireball({0.f, 0.f}, 1.f);
    checkP3(!fireball.isExpired(), "Fireball is not expired initially");
    fireball.update(99.f); // Exceed maxLife (3.f)
    checkP3(fireball.isExpired() && fireball.canBeRemoved(), "Fireball is expired and can be removed after maxLife");

    std::cout << "P3 ItemFactory Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
