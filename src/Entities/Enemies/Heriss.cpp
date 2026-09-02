#include "Entities/Enemies/Heriss.hpp"
#include "Core/Constants.hpp"

Heriss::Heriss(const sf::Vector2f& pos)
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

    entitySprite.setScale({2.0f, 2.0f});
    if (animationComponent) {
        animationComponent->addAnimation("walk", {
            sf::IntRect({290, 14}, {16, 17}),
            sf::IntRect({307, 15}, {16, 15}) });
    }
}

EnemyContactOutcome Heriss::handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection) {
    (void)kind;
    (void)horizontalDirection;
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};

    // Heriss is spiked! Always damages player!
    if (player.isImmortal()) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    player.takeDamage(damage);
    return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
}

void Heriss::onStomped() {
    // Spiked top protection: Stomping Heriss does not kill it
}

void Heriss::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        animationComponent->play("walk", dt);
    }
}

void Heriss::update(float dt) {
    if (dead) return;

    float dirX = facingRight ? 1.f : -1.f;
    move(dirX, 0.f, dt);
    LivingEntity::update(dt);
}
