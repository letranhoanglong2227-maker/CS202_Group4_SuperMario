#include "Entities/Enemies/PeteyPiranha.hpp"
#include "Core/Constants.hpp"
#include <cmath>

PeteyPiranha::PeteyPiranha(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(1);
    setDamage(1);
    setPointsValue(2500);

    setPosition(pos);
    hitbox.setSize({CELL_SIZE, CELL_SIZE * 1.5f});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(0.f, 0.f, 0.f);

    entitySprite.setScale({2.0f, 2.0f});
    if (animationComponent) {
        animationComponent->addAnimation(
            "emerged", {sf::IntRect({205, 37}, {16, 24})});
        animationComponent->addAnimation(
            "retracted", {sf::IntRect({222, 37}, {16, 24})});
    }
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

bool PeteyPiranha::hasPendingProjectile() const {
    return pendingProjectile.has_value();
}

std::optional<ProjectileSpawnRequest> PeteyPiranha::consumePendingProjectile() {
    auto request = pendingProjectile;
    pendingProjectile.reset();
    return request;
}

EnemyContactOutcome PeteyPiranha::handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection) {
    (void)kind;
    (void)horizontalDirection;
    if (dead || !emerged || kind != PlayerEnemyContactKind::Stomp)
        return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};

    if (player.isImmortal()) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    player.takeDamage(damage);
    return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
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
    if (dead) {
        pendingProjectile.reset();
        return;
    }

    emergeTimer += dt;
    if (emergeTimer >= 2.f) {
        emergeTimer = std::fmod(emergeTimer, 2.f);
        emerged = !emerged;
        if (emerged) {
            pendingProjectile = ProjectileSpawnRequest{
                ProjectileKind::PeteySpike,
                {position.x, position.y - CELL_SIZE},
                {0.f, -1.f},
                400.f,
                damage};
        }
    }
    hitbox.setPosition(position);
    entitySprite.setPosition(position);

    updateAnimation(dt);
}
