#include "Entities/Enemies/PeteyPiranha.hpp"
#include "Core/Constants.hpp"

PeteyPiranha::PeteyPiranha(const sf::Vector2f& pos)
    : Enemy(), baseY(pos.y) {
    setHealth(5);
    setDamage(1);
    setPointsValue(2500);

    setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE * 1.5f});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(0.f, 0.f, 0.f);
}

bool PeteyPiranha::isEmerged() const {
    return emerged;
}

void PeteyPiranha::emerge() {
    emerged = true;
}

void PeteyPiranha::retract() {
    emerged = false;
}

void PeteyPiranha::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        setDead(true);
    }
}

void PeteyPiranha::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        animationComponent->play(emerged ? "emerged" : "retracted", dt);
    }
}

void PeteyPiranha::update(float dt) {
    if (dead) return;

    emergeTimer += dt;
    if (emergeTimer >= 2.5f) {
        emergeTimer = 0.f;
        emerged = !emerged;
    }

    if (emerged) {
        position.y = baseY - CELL_SIZE * 1.5f;
    } else {
        position.y = baseY;
    }
    hitbox.setPosition(position);
    entitySprite.setPosition(position);

    updateAnimation(dt);
}
