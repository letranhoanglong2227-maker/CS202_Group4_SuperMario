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
#include "Objects/Items/FireFlower.hpp"

std::unique_ptr<Enemy> EntityFactory::createEnemy(const std::string& type, const sf::Vector2f& position) {
    if (type == "Goomba" || type == "goomba") {
        return std::make_unique<Goomba>(position);
    } else if (type == "Koopa" || type == "koopa") {
        return std::make_unique<Koopa>(position);
    } else if (type == "FlyingKoopa" || type == "flying_koopa") {
        return std::make_unique<FlyingKoopa>(position);
    } else if (type == "Heriss" || type == "heriss" || type == "Spiny") {
        return std::make_unique<Heriss>(position);
    } else if (type == "Bowser" || type == "bowser") {
        return std::make_unique<Bowser>(position);
    } else if (type == "PeteyPiranha" || type == "petey_piranha" || type == "Piranha") {
        return std::make_unique<PeteyPiranha>(position);
    }
    return nullptr;
}

std::unique_ptr<PowerUpObject> EntityFactory::createItem(const std::string& type, const sf::Vector2f& position) {
    if (type == "Mushroom" || type == "mushroom") {
        return std::make_unique<Mushroom>(position);
    } else if (type == "Coin" || type == "coin") {
        return std::make_unique<Coin>(position);
    } else if (type == "FireFlower" || type == "fire_flower" || type == "Flower" || type == "flower") {
        return std::make_unique<FireFlower>(position);
    }
    return nullptr;
}

std::unique_ptr<PlayerManager> EntityFactory::createPlayer(const std::string& type, const sf::Vector2f& position) {
    std::unique_ptr<PlayerManager> player = nullptr;
    if (type == "Luigi" || type == "luigi" || type == "Player2" || type == "2P") {
        player = std::make_unique<Luigi>();
    } else if (type == "Mario" || type == "mario" || type == "Player1" || type == "1P") {
        player = std::make_unique<Mario>();
    }

    if (player) {
        player->setPosition(position);
        player->hitbox.setPosition(position);
    }
    return player;
}
