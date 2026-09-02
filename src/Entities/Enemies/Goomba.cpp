#include "Entities/Enemies/Goomba.hpp"
#include "Core/Constants.hpp"
#include <cmath>

Goomba::Goomba(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(1);
    setDamage(1);
    setPointsValue(300);
    setSpeed(300.f);

    setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(300.f, 2000.f, 0.f);

    // Initial scale
    entitySprite.setScale({2.0f, 2.0f});

    if (animationComponent) {
        animationComponent->addAnimation("walk", { sf::IntRect({1, 45}, {16, 16}), sf::IntRect({18, 45}, {16, 16}) });
        animationComponent->addAnimation("squish", { sf::IntRect({35, 53}, {16, 8}) });
    }
}

EnemyContactOutcome Goomba::handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection) {
    (void)horizontalDirection;
    if (dead || isSquished) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};

    if (kind == PlayerEnemyContactKind::Stomp) {
        onStomped();
        return EnemyContactOutcome{EnemyContactResult::EnemyStomped, pointsValue, -500.f, false};
    }

    if (player.isImmortal()) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    player.takeDamage(damage);
    return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
}

void Goomba::onStomped() {
    if (isSquished) return;
    isSquished = true;
    setStomped(true);
    setDamage(0);
    if (movementComponent) movementComponent->setVelocity({0.f, 0.f});
    hitbox.setSize({CELL_SIZE, CELL_SIZE * 0.5f});
    size = hitbox.getSize();
    position.y += CELL_SIZE * 0.5f; // Anchor feet
    hitbox.setPosition(position);
    entitySprite.setPosition(position);
}

void Goomba::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (stomped) {
            animationComponent->play("squish", dt);
        } else {
            animationComponent->play("walk", dt);
        }
    }
}

void Goomba::update(float dt) {
    if (dead) return;

    if (isSquished) {
        updateAnimation(dt);
        squishTimer += dt;
        if (squishTimer >= 1.0f) {
            setDead(true);
        }
        return;
    }

    float dirX = facingRight ? 1.f : -1.f;
    move(dirX, 0.f, dt);
    LivingEntity::update(dt);
}
