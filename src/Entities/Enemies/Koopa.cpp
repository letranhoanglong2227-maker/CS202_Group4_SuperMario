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
        animationComponent->addAnimation("shell", { sf::IntRect({188, 45}, {16, 14}) });
    }
}

EnemyContactOutcome Koopa::handlePlayerContact(PlayerManager& player, int collisionSide, float horizontalDirection) {
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    
    bool isTop = (collisionSide == 1 || collisionSide == 0);
    
    if (!inShell) {
        if (isTop) {
            onStomped();
            return EnemyContactOutcome{EnemyContactResult::EnemyStomped, 200, -500.f, false};
        } else {
            player.takeDamage(damage);
            return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
        }
    } else {
        if (!shellKicked) {
            float kickDir = (horizontalDirection == 0.f) ? (player.getPosition().x < position.x ? 1.f : -1.f) : horizontalDirection;
            kickShell(kickDir > 0.f);
            return EnemyContactOutcome{EnemyContactResult::ShellKicked, 100, isTop ? -500.f : 0.f, false};
        } else {
            if (isTop) {
                shellKicked = false;
                setSpeed(0.f);
                return EnemyContactOutcome{EnemyContactResult::ShellStopped, 100, -500.f, false};
            } else {
                if (!player.isImmortal()) {
                    player.takeDamage(damage);
                    return EnemyContactOutcome{EnemyContactResult::HazardousShell, 0, 0.f, false};
                }
            }
        }
    }
    return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
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
        float oldH = size.y;
        hitbox.setSize({CELL_SIZE, CELL_SIZE * 0.875f}); // 56.f
        size = hitbox.getSize();
        position.y += (oldH - size.y);
        setSpeed(0.f);
    }
}

void Koopa::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (stomped || inShell) {
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
