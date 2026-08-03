#pragma once

#include <memory>
#include <string>
#include <SFML/System/Vector2.hpp>

#include "Entities/Base/Enemy.hpp"
#include "Entities/Base/Character.hpp"
#include "Objects/Items/PowerUpObject.hpp"

class EntityFactory {
public:
    static std::unique_ptr<Enemy> createEnemy(const std::string& type, const sf::Vector2f& position = { 0.f, 0.f });
    static std::unique_ptr<PowerUpObject> createItem(const std::string& type, const sf::Vector2f& position = { 0.f, 0.f });
    static std::unique_ptr<Character> createPlayer(const std::string& type, const sf::Vector2f& position = { 0.f, 0.f });
};
