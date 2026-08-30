#pragma once

#include <SFML/System/Vector2.hpp>

enum class ProjectileKind {
    Fireball,
    BowserFire,
    PeteySpike
};

struct ProjectileSpawnRequest {
    ProjectileKind type;
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed{ 400.f };
    int damage{ 1 };
};
