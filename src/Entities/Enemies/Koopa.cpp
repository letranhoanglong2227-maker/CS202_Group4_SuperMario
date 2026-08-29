#include "Entities/Enemies/Koopa.hpp"
#include "Core/Constants.hpp"

Koopa::Koopa(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(1);
    setDamage(1);
    setPointsValue(200);
    setSpeed(40.f);

    setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE * 1.5f});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(40.f, 200.f, 0.f);
    
    // Initial scale
    entitySprite.setScale({2.0f, 2.0f});

    if (animationComponent) {
        animationComponent->addAnimation("walk", { sf::IntRect({52, 37}, {16, 24}), sf::IntRect({69, 38}, {16, 23}) });
        animationComponent->addAnimation("squish", { sf::IntRect({188, 45}, {16, 14}) });
    }
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
        if (stomped || inShell) {
            animationComponent->play("squish", dt);
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
