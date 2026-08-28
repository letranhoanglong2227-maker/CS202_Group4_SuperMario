#include "Entities/Enemies/Koopa.hpp"
#include "Core/Constants.hpp"

Koopa::Koopa(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(1);
    setDamage(1);
    setPointsValue(200);
    setSpeed(60.f);

    setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE * 1.5f});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(60.f, 500.f, 0.f);
}

bool Koopa::isInShell() const {
    return inShell;
}

bool Koopa::isShellKicked() const {
    return shellKicked;
}

void Koopa::kickShell(bool toRight) {
    if (!inShell) return;
    shellKicked = true;
    facingRight = toRight;
    setSpeed(shellSpeed);
    movementComponent = std::make_unique<MovementComponent>(shellSpeed, 1000.f, 0.f);
}

void Koopa::onStomped() {
    if (!inShell) {
        inShell = true;
        setStomped(true);
        hitbox.setSize({CELL_SIZE, 56.f});
        setSpeed(0.f);
    } else if (!shellKicked) {
        kickShell(true);
    } else {
        setDead(true);
    }
}

void Koopa::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (inShell) {
            animationComponent->play("shell", dt);
        } else {
            animationComponent->play("walk", dt);
        }
    }
}

void Koopa::update(float dt) {
    if (dead) return;

    if (!inShell || shellKicked) {
        float dirX = facingRight ? 1.f : -1.f;
        move(dirX, 0.f, dt);
    }

    LivingEntity::update(dt);
}
