#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>

struct ProjectileSpawnRequest {
    std::string type;
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed{ 400.f };
    int damage{ 1 };
};
