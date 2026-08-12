#include "Entities/EntityFactory.hpp"
#include "Entities/Players/Mario.hpp"
#include "Entities/Players/Luigi.hpp"
#include "Entities/Enemies/Goomba.hpp"
#include "Entities/Enemies/Koopa.hpp"
#include "Entities/Enemies/FlyingKoopa.hpp"
#include "Entities/Enemies/Heriss.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Entities/Enemies/PeteyPiranha.hpp"
#include "Objects/Items/Coin.hpp"
#include "Objects/Items/Mushroom.hpp"

std::unique_ptr<Enemy> EntityFactory::createEnemy(const std::string& type, const sf::Vector2f& position) {
    std::unique_ptr<Enemy> enemy = nullptr;

    if (type == "Goomba" || type == "goomba") {
        enemy = std::make_unique<Goomba>(position);
    } else if (type == "Koopa" || type == "koopa") {
        enemy = std::make_unique<Koopa>(position);
    } else if (type == "FlyingKoopa" || type == "flying_koopa") {
        enemy = std::make_unique<FlyingKoopa>(position);
    } else if (type == "Heriss" || type == "heriss" || type == "Spiny") {
        enemy = std::make_unique<Heriss>(position);
    } else if (type == "Bowser" || type == "bowser") {
        enemy = std::make_unique<Bowser>(position);
    } else if (type == "PeteyPiranha" || type == "petey_piranha" || type == "Piranha") {
        enemy = std::make_unique<PeteyPiranha>(position);
    } else {
        enemy = std::make_unique<Goomba>(position); // Default fallback
    }

    return enemy;
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
