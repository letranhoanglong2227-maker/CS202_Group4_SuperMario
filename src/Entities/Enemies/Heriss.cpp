#include "Entities/Enemies/Heriss.hpp"
#include "Core/Constants.hpp"

Heriss::Heriss(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(1);
    setDamage(1);
    setPointsValue(200);
    setSpeed(55.f);

    setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(55.f, 500.f, 0.f);
    
    entitySprite.setScale({2.0f, 2.0f});
    if (animationComponent) {
        animationComponent->addAnimation("walk", { sf::IntRect({1, 16}, {16, 16}), sf::IntRect({16, 16}, {16, 16}) }); // Use goomba/spiny placeholder
    }
}

EnemyContactOutcome Heriss::handlePlayerContact(PlayerManager& player, int collisionSide, float horizontalDirection) {
    (void)collisionSide;
    (void)horizontalDirection;
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    
    // Heriss is spiked! Always damages player!
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
