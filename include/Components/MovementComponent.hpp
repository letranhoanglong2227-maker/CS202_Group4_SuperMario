#pragma once
#include <SFML/System/Vector2.hpp>

class MovementComponent {
public:
    MovementComponent(float maxVelocity, float acceleration, float friction);

    const sf::Vector2f& getVelocity() const;
    void setVelocity(const sf::Vector2f& velocity);
    void setVelocity(float x, float y);

    void applyForce(const sf::Vector2f& force);
    void move(float dirX, float dirY, float dt);
    void update(float dt);

private:
    sf::Vector2f velocity;
    float maxVelocity;
    float acceleration;
    float friction;
};

