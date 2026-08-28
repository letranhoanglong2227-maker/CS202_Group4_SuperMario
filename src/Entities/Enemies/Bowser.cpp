#include "Entities/Enemies/Bowser.hpp"
#include "Core/Constants.hpp"

Bowser::Bowser(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(10); // World 3 Boss high HP
    setDamage(2);  // High damage
    setPointsValue(5000);
    setSpeed(40.f);

    setPosition(pos);
    hitbox.setSize({2.f * CELL_SIZE, 140.f});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(40.f, 400.f, 0.f);
}

bool Bowser::isBreathingFire() const {
    return breathingFire;
}

void Bowser::breatheFire() {
    breathingFire = true;
    fireTimer = 0.f;
}

void Bowser::jumpSlam() {
    if (movementComponent) {
        movementComponent->setVelocity(movementComponent->getVelocity().x, -350.f);
        jumpingSlam = true;
    }
}

void Bowser::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        setDead(true);
    }
}

void Bowser::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (breathingFire) {
            animationComponent->play("breathe_fire", dt);
        } else {
            animationComponent->play("walk", dt);
        }
    }
}

void Bowser::update(float dt) {
    if (dead) return;

    attackTimer += dt;
    if (attackTimer >= 3.0f) {
        attackTimer = 0.f;
        if (rand() % 2 == 0) {
            breatheFire();
        } else {
            jumpSlam();
        }
    }

    if (breathingFire) {
        fireTimer += dt;
        if (fireTimer >= 1.0f) {
            breathingFire = false;
        }
    }

    float dirX = facingRight ? 1.f : -1.f;
    move(dirX, 0.f, dt);
    LivingEntity::update(dt);
}
