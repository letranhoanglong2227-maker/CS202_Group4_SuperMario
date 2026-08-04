#include "Entities/EntityFactory.hpp"
#include "Entities/Players/Mario.hpp"
#include "Entities/Players/Luigi.hpp"
#include "Objects/Items/Coin.hpp"
#include "Objects/Items/Mushroom.hpp"

std::unique_ptr<Enemy> EntityFactory::createEnemy(const std::string& type, const sf::Vector2f& position) {
    (void)type;
    (void)position;
    // Will instantiate specific Enemy subclasses in Phase 4
    return nullptr;
}

std::unique_ptr<PowerUpObject> EntityFactory::createItem(const std::string& type, const sf::Vector2f& position) {
    std::unique_ptr<PowerUpObject> item = nullptr;
    if (type == "Mushroom" || type == "mushroom") {
        item = std::make_unique<Mushroom>(position);
    } else {
        item = std::make_unique<Coin>(position);
    }
    return item;
}

std::unique_ptr<Character> EntityFactory::createPlayer(const std::string& type, const sf::Vector2f& position) {
    std::unique_ptr<Character> player = nullptr;
    if (type == "Luigi" || type == "luigi" || type == "Player2" || type == "2P") {
        player = std::make_unique<Luigi>();
    } else {
        player = std::make_unique<Mario>();
    }
    if (player) {
        player->setPosition(position);
        player->hitbox.setPosition(position);
    }
    return player;
}
